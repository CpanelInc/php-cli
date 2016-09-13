#ifndef CLI_H
#define CLI_H

#include <unistd.h>

void cli_get_php_version(char* buf, size_t size, char** argv);
void cli_get_last_file(char* buf, size_t size, char** argv);

#endif /* CLI_H */
