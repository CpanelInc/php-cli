/* ea-php-cli - tests/path_php_cli_yaml_004.c   Copyright 2016 cPanel, Inc. */
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
#include <stdio.h>
#include <string.h>

#include "path.h"

/* mock of char* readlink(char* buf, size_t size) */
char* __wrap_readlink(char* link, char* buf, size_t size) {
    char* return_value = 0;
    if (size > strlen("/usr/bin/lsphp")) {
        return_value = strncpy(buf, "/usr/bin/lsphp", size);
        if (return_value) {
            buf[size-1] = 0;
        }
    }
    return return_value;
}

int main(int argc, char** argv) {
  char path[100] = { 0 };
  char* return_value = 0;
  char* expected_path = "/usr/bin/php_cli.yaml";

  printf("testing path_php_cli_yaml with sufficent buffer\n");

  printf("  calling function path_php_cli_yaml()\n");
  return_value = path_php_cli_yaml(path, 100);

  if (return_value == 0) {
    printf("ERROR: return value %p is 0\n", return_value);
    return 1;
  }
  if (return_value != path) {
    printf("ERROR: return value %p is not %p\n", return_value, path);
    return 1;
  }
  if (strncmp(path, expected_path, 100) != 0) {
    printf("ERROR: path %s is not %s\n", path, expected_path);
    return 1;
  }

  printf("test complete\n");
  return 0;
}
