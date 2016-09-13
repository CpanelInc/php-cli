#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include "cli.h"

#include <unistd.h>
#include <string.h>

#include <stdio.h>

/** Copies the php_version into buf if specified in the command line */
void cli_get_php_version(char* buf, size_t size, char** argv) {
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
    for ( ; lastIndex > 0; lastIndex--) {
       if (access(argv[lastIndex], F_OK) != -1) {
          strncpy(buf, argv[lastIndex], 1024);
          return;
       }
    }
}
