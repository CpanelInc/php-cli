/* ea-php-cli - tests/path_basedir_004.c        Copyright 2016 cPanel, Inc. */
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

int main(int argc, char** argv) {
  char* testcase = "/bob.php";
  char  basedir[PATH_MAX] = { 0 };
  char* return_value = 0;
  char* expected_basedir = "/";

  printf("testing path_basedir on path \"/bob.php\"\n");

  printf("  calling function path_basedir()\n");
  return_value = path_basedir(basedir, testcase, PATH_MAX);

  if (return_value == 0) {
    printf("ERROR: return value %p is 0\n", return_value);
    return 1;
  }
  if (return_value != basedir) {
    printf("ERROR: return value %p is not %p\n", return_value, basedir);
    return 1;
  }
  if (strncmp(basedir, expected_basedir, 20) != 0) {
    printf("ERROR: basedir %s is not %s\n", basedir, expected_basedir);
    return 1;
  }

  printf("test complete\n");
  return 0;
}
