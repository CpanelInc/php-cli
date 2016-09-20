/* ea-php-cli - tests/cli_config_read_cli_config_001.c  Copyright 2016 cPanel, Inc. */
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

#include "cli-config.h"

int unauthorized_call__xstat = 0;

int __wrap___xstat(int ver, const char* path, struct stat* buf) {
   unauthorized_call__xstat = 1;
   return -1;
}

int unauthorized_call_fopen = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
   unauthorized_call_fopen = 1;
   return 0;
}

int unauthorized_call_fclose = 0;

int __wrap_fclose(FILE* file) {
   unauthorized_call_fclose = 1;
   return 0;
}

int main(int argc, char** argv) {
  struct cli_config* testcase = 0;
  char   file[1024] = "unset";
  int    return_code = 0;

  printf("testing cli_config_read_cli_config on null pointer\n");
  printf("  calling cli_config_read_cli_config(\"%s\", %d, (struct cli_config*)0)\n", file, 1024);

  return_code = cli_config_read_cli_config(file, 1024, testcase);

  if (unauthorized_call__xstat) {
    printf("ERROR: attempt to lookup path occurred\n");
    return 1;
  } else {
    printf("  stat not called\n");
  }

  if (unauthorized_call_fopen) {
    printf("ERROR: attempt to open path occurred\n");
    return 1;
  } else {
    printf("  fopen not called\n");
  }

  if (unauthorized_call_fclose) {
    printf("ERROR: attempt to close path occurred\n");
    return 1;
  } else {
    printf("  fclose not called\n");
  }

  if (return_code == 0) {
    printf("ERROR: return code %d is zero\n", return_code);
    return 1;
  } else {
    printf("  return code %d is not zero\n", return_code);
  }

  printf("test complete\n");
  return 0;
}
