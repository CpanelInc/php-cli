/* ea-php-cli - tests/htaccess_get_php_version_from_file_008.c  Copyright 2016 cPanel, Inc. */
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

#include "cli.h"
#include "htaccess.h"

/* act like the path exists */
int __wrap___xstat(int ver, const char* path, struct stat* buf) {
    buf->st_mode = S_IFREG;
    return 0;
}

int fopen_called = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
    fopen_called = 1;
    return (FILE*)5;
}

int fgets_call_count = 0;

/* file contains an AddType directive */
char* __wrap_fgets(char* buf, int size, FILE* file) {
    char* file_contents[] = {
        "one",
        "two",
        "AddType application/x-httpd-ea-php70 .php .php5 .php6 .php7",
        "four"
    };

    fgets_call_count++;

    if (fgets_call_count > (int)(sizeof(file_contents)/sizeof(file_contents[0]))) {
       return 0;
    }
    strncpy(buf, file_contents[fgets_call_count-1], size);
    return buf;
}

int fclose_called = 0;

/* */
int __wrap_fclose(FILE* file) {
    fclose_called = 1;
    return 0;
}

int main(int argc, char** argv) {
  char testcase[1024] = "/some/path/.htaccess";
  char version[8] = "junk";

  char expected_version[8] = "70";
  char expected_fgets_call_count = 3;

  printf("testing htaccess_get_php_version_from_file on regular file\n");
  printf("  calling htaccess_get_php_version_from_file(\"%s\", %d, \"%s\", %d)\n",
         version, 8, testcase, 1024);
  htaccess_get_php_version_from_file(version, 8, testcase, 1024);

  if (fopen_called == 0) {                                              
    printf("ERROR: no attempt to open file\n");                         
    return 1;                                                           
  } else {                                                              
    printf("  fopen called\n");                                         
  }  

  if (fgets_call_count != expected_fgets_call_count) {
    printf("ERROR: fgets called %d times, expected %d\n", fgets_call_count, expected_fgets_call_count);
    return 1;
  } else {
    printf("  fgets call count is correct\n");
  }

  if (fclose_called == 0) {                                              
    printf("ERROR: no attempt to close file\n");                         
    return 1;                                                           
  } else {                                                              
    printf("  fclose called\n");                                         
  }  

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
