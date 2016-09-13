#ifndef STRATEGY_H
#define STRATEGY_H

/* Contains logic routines for finding the preferred item */

#include <unistd.h>

#include "cli-config.h"

void strategy_get_php_bin(char* buffer, size_t size, struct cli_config* cli_config, char* php_version);
void strategy_get_lsphp_bin(char* buffer, size_t size, struct cli_config* cli_config, char* php_version);

/* Typically implemented by the cli tool */
void get_bin_php_default_pattern(char* buffer, size_t size);

#endif /* STRATEGY_H */

