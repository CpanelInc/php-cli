#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include "strategy.h"

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>

void strategy_get_php_bin(char* buffer, size_t size,
                 struct cli_config* cli_config, char* php_version) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;

    memset(buffer, 0, size);
    if (cli_config == 0 || php_version == 0 || php_version[0] == 0) {
        return;
    }
    if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, php_version);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, php_version);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}

void strategy_get_lsphp_bin(char* buffer, size_t size,
                   struct cli_config* cli_config, char* php_version) {
    char        default_pattern[1024] = { 0 };
    struct stat path_stat;
    memset(buffer, 0, size);
    if (cli_config == 0 || php_version == 0 || php_version[0] == 0) {
        return;
    }
    if (cli_config->lsphp_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->lsphp_bin_pattern, php_version);
    } else if (cli_config->php_bin_pattern[0] != 0) {
        snprintf(buffer, size, cli_config->php_bin_pattern, php_version);
    } else {
        get_bin_php_default_pattern(default_pattern, 1024);
        snprintf(buffer, size, default_pattern, php_version);
    }
    if (stat(buffer, &path_stat) != 0 ||
        S_ISREG(path_stat.st_mode) == 0 ||
        access(buffer, X_OK) != 0) {
      memset(buffer, 0, size);
    }
}
