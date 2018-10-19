/* ea-php-cli - src/php-cli.c                   Copyright 2017 cPanel, Inc. */
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
#include "paths-config.h"
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

char*   get_default_php_package(char* ea_php_config) {
    char*       php_package_string = NULL;
    key_pair_t* php_config_head = NULL;

    if (!file_exists (ea_php_config)) {
        fprintf( stderr, "EasyApache PHP config file not found: %s\n", ea_php_config);
        exit (1);
    }

    php_config_head = parse_yaml_file (ea_php_config);

    php_package_string = get_value (php_config_head, "default"); /* cpanel 11.54+ key */
    if (!php_package_string) {
        php_package_string = get_value (php_config_head, "phpversion"); /* cpanel 11.52 key */
    }

    if (php_package_string == NULL) {
        fputs ("Default PHP not defined in EasyApache configuration file\n", stderr);
        exit (1);
    }

    return php_package_string;
}

void get_bin_php_default_pattern(char* buffer, size_t size) {
    strncpy(buffer, "%s/%s/root/usr/bin/php", size);
}

int main(int argc, char* argv[]) {
    char                php_package[20]      = { 0 };
    char                php_file[1024]       = { 0 };
    char                reference_path[1024] = { 0 };
    char                php_conf_file[1024]  = { 0 };
    char                php_bin[1024]        = { 0 };
    struct cli_config   cli_config;
    struct paths_config paths_config;
    int                 has_verbose = 0;

    int     i;
    key_pair_t* command_line = NULL;
    int     xargc;
    char    **xargv;

    cli_config_load(&cli_config);
    paths_config_load(&paths_config);
    strategy_get_php_conf_file(php_conf_file, 1024, &cli_config, &paths_config);

    /* all this goes in get_php_package */
    cli_get_php_package(php_package, 20, argv);
    if (php_package[0] == 0) {
      cli_get_last_file(php_file, 1024, argv);
      cli_get_reference_path(reference_path, 1024, argv);
      if (php_file[0] == 0) {
          strncpy(php_file, "fake.php", 1024);
      }

      if (reference_path[0] != 0) {
          strncat(reference_path, "/.", 1024); /* the last piece is stripped off, so we need to add a dummy piece */
          path_get_htaccess_php_package(php_package, 20, reference_path, 1024);
      }
      else {
          path_get_htaccess_php_package(php_package, 20, php_file, 1024);
      }
    }
    strategy_get_php_bin(php_bin, 1024, &cli_config, php_package);
    if (php_bin[0] == 0) {
        if (php_package[0] != 0) {
            fprintf(stderr, "Configured PHP version %s does not have a CLI binary installed, using default PHP version\n", php_package);
        }
        strncpy(php_package, get_default_php_package(php_conf_file), 20);
        strategy_get_php_bin(php_bin, 1024, &cli_config, php_package);
    }
    if (php_bin[0] == 0) {
        fprintf(stderr, "Default PHP version %s does not have a CLI binary installed, exiting\n", php_package);
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
            if (!strcmp (argv [i], "-ea_reference_dir")) {
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
       printf("ea-php-cli Copyright 2017 cPanel, Inc.\n");
    }
    i = execv (&php_bin [0], xargv);
    printf ("php_cli unable to execute php :%d: (%d) :%s:\n", i, errno, strerror (errno));
    printf ("when attempting to run (%s)\n", &php_bin [0]);

    return (0);
}

