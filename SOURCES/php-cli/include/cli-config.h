/* ea-php-cli - include/cli-config.h            Copyright 2016 cPanel, Inc. */
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

#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include <unistd.h>

#define CLI_CONFIG_EA_PHP_CONFIG_SIZE 1024
#define CLI_CONFIG_TESTING_ROOT_DIR_SIZE 1024
#define CLI_CONFIG_PHP_BIN_PATTERN_SIZE 1024
#define CLI_CONFIG_LSPHP_BIN_PATTERN_SIZE 1024

struct cli_config {
  char ea_php_config[CLI_CONFIG_EA_PHP_CONFIG_SIZE];
  char testing_root_dir[CLI_CONFIG_TESTING_ROOT_DIR_SIZE];
  char php_bin_pattern[CLI_CONFIG_PHP_BIN_PATTERN_SIZE];
  char lsphp_bin_pattern[CLI_CONFIG_LSPHP_BIN_PATTERN_SIZE];
};

int  cli_config_read_cli_config(char* path, size_t size, struct cli_config* buf);
void cli_config_load(struct cli_config* buf);

#endif /* CLI_CONFIG_H */
