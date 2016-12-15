/* ea-php-cli - tests/strategy_get_php_conf_file_004.c  Copyright 2016 cPanel, Inc. */
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
#include "paths-config.h"

int get_bin_php_default_pattern_called = 0;

void get_bin_php_default_pattern(char* buffer, size_t size) {
   get_bin_php_default_pattern_called = 1;
   strncpy(buffer, "/my/alternate/%s/bin/php", size);
}

int __wrap___xstat(int ver, const char* path, struct stat* buf) {
  buf->st_mode = S_IFREG;
  return 0;
}

int __wrap_access(const char* pathname, int flags) {
  return 0;
}

int main(int argc, char** argv) {
  struct cli_config cli_config;
  struct paths_config paths_config;
  char php_conf_file[1024] = "junk";

  printf("testing strategy_get_php_conf_file with cli_config set value\n");
  printf("  calling strategy_get_php_conf_file(\"%s\", %d, &cli_config, &paths_config)\n", php_conf_file, 1024);
  strategy_get_php_conf_file(php_conf_file, 1024, &cli_config, &paths_config);

  if (get_bin_php_default_pattern_called) {
    printf("ERROR: get_bin_php_default_pattern was called\n");
    return 1;
  } else {
    printf("  get_bin_php_default_pattern not called\n");
  }
    
  if (strncmp(php_conf_file, "/etc/cpanel/ea4/php.conf", 1024) != 0) {
    printf("ERROR: php_conf_file \"%s\" is not \"/etc/cpanel/ea4/php.conf\"\n", php_conf_file);
    return 1;
  } else {
    printf("  php_conf_file \"%s\" is set correctly to \"/etc/cpanel/ea4/php.conf\"\n", php_conf_file);
  }

  printf("test complete\n");
  return 0;
}
