/* ea-php-cli - tests/cli_get_php_package_003.c  Copyright 2017 cPanel, Inc. */
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

#include <stdio.h>
#include <string.h>

#include "cli.h"

int main(int argc, char** argv) {
  char* testcase[] = { "-ea_php", "54", 0 };
  char version[8] = "junk";

  char* expected_version = "ea-php54";

  printf("testing cli_get_php_package on \"-ea_php 54\"\n");
  printf("  calling cli_get_php_package(\"%s\", %d, @{\"-ea_php\", \"54\", 0})\n",
         version, 8);

  cli_get_php_package(version, 8, testcase);

  if (strnlen(version, 8) == 0 || 
      strncmp(expected_version, version, 8) != 0) {
    printf("ERROR: version \"%s\" is not \"%s\"\n", 
           version, expected_version);
    return 1;
  } else {
    printf("  version \"%s\" is \"%s\"\n", 
           version, expected_version);
  }

  printf("test complete\n");
  return 0;
}
