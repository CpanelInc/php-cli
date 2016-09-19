/* ea-php-cli - src/strategy.c                  Copyright 2016 cPanel, Inc. */
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

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

void strategy_get_php_bin(char* buffer, size_t size,
                 struct cli_config* cli_config, char* php_version) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;

    memset(buffer, 0, size);
    if (cli_config == 0 || php_version == 0 || php_version[0] == 0) {
        return;
    }
    if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, php_version);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, php_version);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}

void strategy_get_lsphp_bin(char* buffer, size_t size,
                   struct cli_config* cli_config, char* php_version) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;
    memset(buffer, 0, size);
    if (cli_config == 0 || php_version == 0 || php_version[0] == 0) {
        return;
    }
    if (cli_config->lsphp_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->lsphp_bin_pattern, php_version);
    } else if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, php_version);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, php_version);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}
