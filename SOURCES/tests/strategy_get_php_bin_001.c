/* ea-php-cli - tests/strategy_get_php_bin_001.c  Copyright 2017 cPanel, Inc. */
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

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "strategy.h"

int get_bin_php_default_pattern_called = 0;

void get_bin_php_default_pattern(char* buffer, size_t size) {
   get_bin_php_default_pattern_called = 1;
   strncpy(buffer, "/my/alternate/%s/bin/php", size);
}

int main(int argc, char** argv) {
  struct cli_config* cli_config = 0;
  char php_package[8] = "30";
  char php_bin[1024] = "junk";

  printf("testing strategy_get_php_bin on null cli_config\n");
  printf("  calling strategy_get_php_bin(\"%s\", %d, (cli_config*)0, %s)\n", php_bin, 1024, php_package);
  strategy_get_php_bin(php_bin, 1024, cli_config, php_package);

  if (get_bin_php_default_pattern_called) {
    printf("ERROR: get_bin_php_default_pattern was called\n");
    return 1;
  } else {
    printf("  get_bin_php_default_pattern not called\n");
  }
    
  if (strnlen(php_bin, 1024) != 0) {
    printf("ERROR: php_bin \"%s\" is not empty\n", php_bin);
    return 1;
  } else {
    printf("  php_bin \"%s\" is empty\n", php_bin);
  }

  printf("test complete\n");
  return 0;
}
