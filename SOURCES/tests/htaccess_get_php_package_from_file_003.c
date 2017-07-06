/* ea-php-cli - tests/htaccess_get_php_package_from_file_003.c  Copyright 2017 cPanel, Inc. */
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

#include <errno.h>
#include "cli.h"
#include "htaccess.h"

/* act like the path does not exist */
int __wrap___xstat(int ver, const char* path, struct stat* buf) {
   return -1;
}

int unauthorized_call_fopen = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
  unauthorized_call_fopen = 1;
  return 0;
}

int unauthorized_call_fgets = 0;

char* __wrap_fgets(char* buf, int size, FILE* file) {
    unauthorized_call_fgets = 1;
    return 0;
}

int unauthorized_call_fclose = 0;

int __wrap_fclose(FILE* file) {
    unauthorized_call_fclose = 1;
    return 0;
}

int main(int argc, char** argv) {
  char testcase[1024] = "/does/not/exist/.htaccess";
  char version[8] = "junk";

  printf("testing htaccess_get_php_package_from_file on empty string\n");
  printf("  calling htaccess_get_php_package_from_file(\"%s\", %d, \"%s\", %d)\n",
         version, 8, testcase, 1024);
  htaccess_get_php_package_from_file(version, 8, testcase, 1024);

  if (unauthorized_call_fopen) {
    printf("ERROR: attempt to open directory occurred\n");
    return 1;
  } else {
    printf("  fopen not called\n");
  }

  if (unauthorized_call_fgets) {
    printf("ERROR: attempt to read from directory occurred\n");
    return 1;
  } else {
    printf("  fgets not called\n");
  }

  if (unauthorized_call_fclose) {
    printf("ERROR: attempt to close file occurred\n");                         
    return 1;                                                           
  } else {                                                              
    printf("  fclose not called\n");                                         
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
