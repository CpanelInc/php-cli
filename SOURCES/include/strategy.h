/* ea-php-cli - include/strategy.h              Copyright 2016 cPanel, Inc. */
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

#ifndef STRATEGY_H
#define STRATEGY_H

/* Contains logic routines for finding the preferred item */

#include <unistd.h>

#include "cli-config.h"
#include "paths-config.h"

void strategy_get_php_bin(char* buffer, size_t size, struct cli_config* cli_config, char* php_version);
void strategy_get_lsphp_bin(char* buffer, size_t size, struct cli_config* cli_config, char* php_version);
void strategy_get_php_conf_file(char* buffer, size_t size, struct cli_config* cli_config, struct paths_config* paths_config);

/* Typically implemented by the cli tool */
void get_bin_php_default_pattern(char* buffer, size_t size);

#endif /* STRATEGY_H */
