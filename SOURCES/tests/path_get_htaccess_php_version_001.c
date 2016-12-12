/* ea-php-cli - tests/path_get_htaccess_php_version_001.c  Copyright 2016 cPanel, Inc. */
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

int unauthorized_get_from_file = 0;

void __wrap_htaccess_get_php_version_from_file(char* buf, size_t size, char* path, size_t path_size) {
  unauthorized_get_from_file = 1;
}

int main(int argc, char** argv) {
  char* testcase = 0;
  char  version[8] = "junk";

  printf("testing path_get_htaccess_php_version on null pointer\n");
  printf("  calling function path_get_htaccess_php_version(\"%s\", %d, \"%s\", %d)\n", version, 8, testcase, 1024);

  path_get_htaccess_php_version(version, 8, testcase, 1024);

  if (unauthorized_get_from_file) {
    printf("ERROR: attempt to htaccess_get_php_version_from_file occurred\n");
    return 1;
  } else {
    printf("  htaccess_get_php_version_from_file not called\n");
  }

  if (strnlen(version, 8) != 0) {
    printf("ERROR: version \"%s\" is not empty\n", version);
    return 1;
  } else {
    printf("  version \"%s\" is empty\n", version);
  }

  printf("test complete\n");
  return 0;
}
