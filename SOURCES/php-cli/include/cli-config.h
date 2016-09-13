#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include <unistd.h>

#define CLI_CONFIG_EA_PHP_CONFIG_SIZE 1024
#define CLI_CONFIG_TESTING_ROOT_DIR_SIZE 1024
#define CLI_CONFIG_PHP_BIN_PATTERN_SIZE 1024
#define CLI_CONFIG_LSPHP_BIN_PATTERN_SIZE 1024

struct cli_config {
  char ea_php_config[CLI_CONFIG_EA_PHP_CONFIG_SIZE];
  char testing_root_dir[CLI_CONFIG_TESTING_ROOT_DIR_SIZE];
  char php_bin_pattern[CLI_CONFIG_PHP_BIN_PATTERN_SIZE];
  char lsphp_bin_pattern[CLI_CONFIG_LSPHP_BIN_PATTERN_SIZE];
};

int  cli_config_read_cli_config(char* path, size_t size, struct cli_config* buf);
void cli_config_load(struct cli_config* buf);

#endif /* CLI_CONFIG_H */

