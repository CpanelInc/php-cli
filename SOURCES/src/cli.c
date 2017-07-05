/* ea-php-cli - src/cli.c                       Copyright 2017 cPanel, Inc. */
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

#include "cli.h"

#include <unistd.h>
#include <string.h>

#include <stdio.h>

/** Copies the php_package into buf if specified in the command line */
void cli_get_php_package(char* buf, size_t size, char** argv) {
    memset(buf, 0, size);
    if (argv == 0) {
        return;
    }
    char** index = argv;
    while (*index != 0 && *(index+1) != 0) {
        if (!strncmp(*index, "-ea_php", 10)) {
          strncpy(buf, *(index+1), size);
          return;
        }
        index++; 
    }
    return;
}

/** Copies the last argv entry into buf if it is a file */
void cli_get_last_file(char* buf, size_t size, char** argv) {
    memset(buf, 0, size);
    if (argv == 0) {
       return;
    }
    char** index = argv;
    int lastIndex = -1;
    while (*index != 0) {
       lastIndex++;
       index++;
    }
    /* stop before the executable */
    for ( ; lastIndex > 0; lastIndex--) {
       if (access(argv[lastIndex], F_OK) != -1) {
          strncpy(buf, argv[lastIndex], 1024);
          return;
       }
    }
}

int cli_get_verbose(int* buf, char** argv) {
    char** index = argv;
    if (buf == 0) {
        return 1;
    }
    *buf = 0;
    if (argv == 0 || *index == 0) {
        return 0;
    }
    index++; /* skip the executable argv[0] */
    while (*buf == 0 && *index != 0) {
        if (strncmp("-v", *index, 3) == 0) {
           *buf = 1;
        }
        index++;
    }
    return 0;
}
