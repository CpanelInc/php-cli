/* ea-php-cli - src/strategy.c                  Copyright 2017 cPanel, Inc. */
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

#include "strategy.h"
char TEST_SCL_PREFIX[255];

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "paths-config.h"

#define MAXPATH 1024

void strategy_get_scl_prefix_for_package(char **scl_prefix, char* php_package)
{

   *scl_prefix = malloc(MAXPATH + 1);

   if (TEST_SCL_PREFIX != NULL && TEST_SCL_PREFIX != "" && strlen(TEST_SCL_PREFIX) != 0) {
       strcpy(*scl_prefix, TEST_SCL_PREFIX);
       return;
   }

   FILE *fh;

   char buff[255];
   char fname[MAXPATH + 1];
   snprintf(fname, MAXPATH, "/etc/scl/prefixes/%s", php_package);

   if( access( fname, F_OK ) != -1 ) {
       fh = fopen(fname, "r");
       if(!fscanf(fh, "%s", buff))
       {
           scl_prefix = NULL;
       }
       fclose(fh);
       strcpy(*scl_prefix, buff);
   }

   // printf("File \"%s\" contained \"%s\"\n", fname, *scl_prefix);
}

void strategy_get_php_bin(char* buffer, size_t size,
                 struct cli_config* cli_config, char* php_package) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;

    char *scl_prefix;
    strategy_get_scl_prefix_for_package(&scl_prefix, php_package);

    memset(buffer, 0, size);
    if (cli_config == 0 || php_package == 0 || php_package[0] == 0) {
        return;
    }
    if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, scl_prefix, php_package);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, scl_prefix, php_package);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}

void strategy_get_lsphp_bin(char* buffer, size_t size,
                   struct cli_config* cli_config, char* php_package) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;
    char *scl_prefix;
    strategy_get_scl_prefix_for_package(&scl_prefix, php_package);

    memset(buffer, 0, size);
    if (cli_config == 0 || php_package == 0 || php_package[0] == 0) {
        return;
    }
    if (cli_config->lsphp_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->lsphp_bin_pattern, scl_prefix, php_package);
    } else if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, scl_prefix, php_package);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, scl_prefix, php_package);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}

void strategy_get_php_conf_file(char* buffer, size_t size,
                                struct cli_config* cli_config,
                                struct paths_config* paths_config) {
    struct stat file_stat;

    memset(buffer, 0, size);
    if (cli_config == 0 || paths_config == 0 ) {
        return;
    }
    if (cli_config->ea_php_config[0] != 0) {
        strncpy(buffer, cli_config->ea_php_config, size);
    } else if (stat("/etc/cpanel/ea4/php.conf", &file_stat) == 0 &&
               S_ISREG(file_stat.st_mode) != 0 &&
               access("/etc/cpanel/ea4/php.conf", R_OK) == 0) {
        strncpy(buffer, "/etc/cpanel/ea4/php.conf", size);
    } else {
        snprintf(buffer, size, "%s/php.conf.yaml", paths_config->dir_conf);
    }
    return;
}
