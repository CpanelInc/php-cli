/* ea-php-cli - src/php-cgi.c                   Copyright 2016 cPanel, Inc. */
/*                                                     All rights Reserved. */
/* copyright@cpanel.net                                   http://cpanel.net */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <yaml.h>

#include "cli.h"
#include "cli-config.h"
#include "path.h"
#include "php-cli.h"
#include "strategy.h"

/* TODO: when this is put into the main repo:
 * Add a comment in Cpanel::Config::Apache that if its logic changes
 * to be sure to update <WHEREVER THIS LIVES IN /usr/local/cpanel HERE>
 * accordingly also.
 */

char            apache_conf_dir[1024] = { 0 };

int file_exists(char* fname) {
    if (access(fname, F_OK) != -1) {
        return 1;
    }
    return 0;
}

void load_paths_conf() {
    char*       paths_conf = NULL;
    key_pair_t* paths_conf_head = NULL;

/* TODO when this is put into the main repo:
 *   Add a comment in Cpanel::Config::Apache that if its logic changes to be sure
 *   to update <WHEREVER THIS LIVES IN /usr/local/cpanel HERE> accordingly also.
 */

    paths_conf = get_string_copy("/etc/cpanel/ea4/paths.conf");
    if (file_exists(paths_conf)) {
        char    *ptr;

        paths_conf_head = parse_conf_file (paths_conf);
        ptr = get_value(paths_conf_head, "dir_conf");
        if (ptr != NULL) {
            strcpy(apache_conf_dir, ptr);
        }
    } else {
        strncpy(apache_conf_dir, "/etc/apache2/conf.d", 1024);
    }

    return;
}

void set_defaults(struct cli_config* cli_config) {
    /* ea_php_config doesn't exist, try the EA4 (11.54+) location */
    if (cli_config->ea_php_config[0] == 0 && file_exists("/etc/cpanel/ea4/php.conf")) {
        strncpy(cli_config->ea_php_config, "/etc/cpanel/ea4/php.conf", 1024);
    }
    /* Does 11.52 always have a paths.conf? */
    /* ea_php_config still doesn't exist, try the 11.52 location */
    if (cli_config->ea_php_config[0] == 0) {
          load_paths_conf();
          strncpy(cli_config->ea_php_config, apache_conf_dir, 1024);
          /* on a default EA4 (11.60) resolves to /etc/apache2/conf.d/php.conf.yaml */
          strncat(cli_config->ea_php_config, "/php.conf.yaml", 15);
    }
    /* no explicit lsphp_bin_pattern, set an EA4 default */
    if (cli_config->lsphp_bin_pattern[0] == 0) {
        strncpy(cli_config->lsphp_bin_pattern, "/opt/cpanel/ea-php%s/root/usr/bin/lsphp", 40);
    }
}

char*   get_default_php_version(char* ea_php_config) {
    char*       php_version_string = NULL;
    key_pair_t* php_config_head = NULL;
    int         len;

    if (!file_exists(ea_php_config)) {
        fprintf( stderr, "EasyApache PHP config file not found: %s\n", ea_php_config);
        exit (1);
    }

    php_config_head = parse_yaml_file (ea_php_config);

    php_version_string = get_value (php_config_head, "default"); /* cpanel 11.54+ key */
    if (!php_version_string) {
        php_version_string = get_value (php_config_head, "phpversion"); /* cpanel 11.52 key */
    }

    /* last 2 chars are the version number */

    if (php_version_string == NULL) {
        fputs ("Default PHP not defined in EasyApache configuration file\n", stderr);
        exit (1);
    }
  
    len = strlen (php_version_string); 
    return &php_version_string [len - 2];
}

void get_bin_php_default_pattern(char* buffer, size_t size) {
    strncpy(buffer, "/opt/cpanel/ea-php%s/root/usr/bin/php-cgi", size);
}

int main(int argc, char* argv[]) {
    char              php_version[8] = { 0 };
    char              php_file[1024] = { 0 };
    char              php_bin[1024]  = { 0 };
    struct cli_config cli_config;
    int               has_verbose = 0;

    int     i;

    key_pair_t* command_line = NULL;
    int     xargc;
    char    **xargv;

    cli_config_load(&cli_config);
    set_defaults(&cli_config);

    /* all this goes in get_php_version */
    cli_get_php_version(php_version, 8, argv);
    if (php_version[0] == 0) {
      cli_get_last_file(php_file, 1024, argv);
      if (php_file[0] == 0) {
          strncpy(php_file, "fake.php", 1024);
      }
      path_get_htaccess_php_version(php_version, 8, php_file, 1024);
    }
    strategy_get_php_bin(php_bin, 1024, &cli_config, php_version);
    if (php_bin[0] == 0) {
        if (php_version[0] != 0) {
            fprintf(stderr, "Configured PHP version %s not installed, using default PHP version\n", php_version);
        }
        strncpy(php_version, get_default_php_version(cli_config.ea_php_config), 8);
        strategy_get_php_bin(php_bin, 1024, &cli_config, php_version);
    }
    if (php_bin[0] == 0) {
        fprintf(stderr, "Default PHP version %s not installed, exiting\n", php_version);
    }

    /* now process command line parameters */

    {
        key_pair_t      *next = NULL;
        key_pair_t      *current = NULL;

        xargc = 0;
        for (i = 0; i < argc; ++i) {
            if (!strcmp (argv [i], "-ea_php")) {
                if ((i + 1) < argc) {
                    i++; /* double increment */
                    continue;
                }
            }

            current = alloc_key_pair ();
            current->key = get_string_copy ("place_holder");

            if (i == 0)
                current->value = get_string_copy (&php_bin [0]);
            else
                current->value = get_string_copy (argv [i]);

            xargc ++;

            if (command_line == NULL) {
                command_line = current;
            } else {
                next->next = current;
            }

            next = current;
        }

        xargv = malloc ((xargc + 1) * sizeof (char*));
        for (i = 0, next = command_line; i < xargc; ++i) {
            xargv [i] = get_string_copy (next->value);
            next = next->next;
        }

        xargv [i] = NULL;
    }

    cli_get_verbose(&has_verbose, xargv);
    if (has_verbose) {
       printf("ea-php-cli Copyright 2016 cPanel, Inc.\n");
    }
    i = execv (&php_bin [0], xargv);
    printf ("php_cli unable to execute php :%d: (%d) :%s:\n", i, errno, strerror (errno));
    printf ("when attempting to run (%s)\n", &php_bin [0]);

    return (0);
}
