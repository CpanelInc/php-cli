/* ea-php-cli - tests/path_get_htaccess_php_package_006.c  Copyright 2017 cPanel, Inc. */
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

int get_from_file_call_count = 0;

void __wrap_htaccess_get_php_package_from_file(char* buf, size_t size, char* path, size_t path_size) {
   get_from_file_call_count++;
   memset(buf, 0, size);
   if (strncmp(path, "/.htaccess", path_size) == 0) {
     strncpy(buf, "56", size);
   }
}

int main(int argc, char** argv) {
  char  testcase[1024] = "/a/deep/test/test.php";
  char  version[8] = "junk";

  char  expected_version[8] = "56";
  int   expected_get_from_file_call_count = 4;

  printf("testing path_get_htaccess_php_package with php version 56 in /.htaccess path for \"%s\"\n", testcase);

  printf("  calling function path_get_htaccess_php_package(\"%s\", %d, \"%s\", %d)\n", version, 8, testcase, 1024);
  path_get_htaccess_php_package(version, 8, testcase, 1024);

  if (get_from_file_call_count != expected_get_from_file_call_count) {
    printf("ERROR: htaccess_get_php_package_from_file called %d times, expected %d\n",
           get_from_file_call_count, expected_get_from_file_call_count);
    return 1;
  } else {
    printf("  htaccess_get_php_package_from_file call count is correct\n");
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
