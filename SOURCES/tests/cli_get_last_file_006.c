/* ea-php-cli - tests/cli_get_last_file_006.c   Copyright 2016 cPanel, Inc. */
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

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"

int executable_checked = 0;

/* mock of int access(const char* pathname, int flags) */
int __wrap_access(const char* pathname, int flags) {
    if (strcmp("executable", pathname) == 0) {
       executable_checked = 1;
    }
    return -1;
}

int main(int argc, char** argv) {
  char* testcase[] = { "executable", "first", "second", "third", "fourth", 0 };
  char  file[1024] = "unset";

  printf("testing cli_get_last_file on \"first second third fourth\" where none are files\n");
  printf("  calling cli_get_last_file(\"%s\", %d, @{ \"first\", \"second\", \"thrid\", \"fourth\", 0})\n", file, 1024);

  cli_get_last_file(file, 1024, testcase);

  if (executable_checked) {
    printf("ERROR: executable was checked as a last file\n");
    return 1;
  } else {
    printf("  executable was not checked as a last file\n");
  }

  if (strnlen(file, 1024) != 0) {
    printf("ERROR: file \"%s\" is not empty\n", file);
    return 1;
  } else {
    printf("  file \"%s\" is empty\n", file);
  }

  printf("test complete\n");
  return 0;
}
