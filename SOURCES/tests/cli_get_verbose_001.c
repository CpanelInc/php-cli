/* ea-php-cli - tests/cli_get_verbose_001.c     Copyright 2016 cPanel, Inc. */
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
  char* testcase[] = { "executable", "first", "second", "third", "fourth", 0 };

  int* has_verbose = 0;
  int  return_value = 0;

  printf("testing cli_verbose on null buffer\n");
  printf("  calling cli_verbose(%p, &{\"executable\", \"first\", \"second\", \"third\", \"fourth\", 0 })\n", has_verbose);

  return_value = cli_get_verbose(has_verbose, testcase);

  /* basically the program should not crash */
  if (return_value == 0) {
    printf("ERROR: return value %d was zero\n", return_value);
    return 1;
  } else {
    printf("  return value %d was not zero\n", return_value);
  }

  printf("test complete\n");
  return 0;
}

