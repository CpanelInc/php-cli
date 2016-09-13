#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

#include "path.h"

/* mock of char* readlink(char* buf, size_t size) */
char* __wrap_readlink(char* buf, size_t size) {
    char* return_value = 0;
    if (size > strlen("/usr/bin/php")) {
        return_value = strncpy(buf, "/usr/bin/php", size);
        if (return_value) {
            buf[size-1] = 0;
        }
    }
    return return_value;
}

int main(int argc, char** argv) {
  char* path = 0;
  char* return_value = 0;
  char* expected_path = 0;

  printf("testing path_php_cli_yaml on null pointer\n");

  printf("  calling function path_php_cli_yaml()\n");
  return_value = path_php_cli_yaml(path, 0);

  if (return_value != 0) {
    printf("ERROR: return value %p is not 0\n", return_value);
    return 1;
  }
  if (path != expected_path) {
    printf("ERROR: path value %p is not %p\n", path, expected_path);
    return 1;
  }

  printf("test complete\n");
  return 0;
}
