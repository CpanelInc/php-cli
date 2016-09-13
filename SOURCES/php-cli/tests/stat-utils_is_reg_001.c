#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "stat-utils.h"

int main(int argc, char** argv) {
  char** testcase = 0;
  char version[8] = "junk";

  printf("testing stat_is_reg on null pointer\n");
  printf("  calling cli_get_php_version(\"%s\", %d, %s)\n", version, 8, testcase);
  cli_get_php_version(version, 8, testcase);

  if (strnlen(version, 8) != 0) {
    printf("ERROR: version %s is not empty\n", version);
    return 1;
  } else {
    printf("  version \"%s\" is empty\n", version);
  }

  printf("test complete\n");
  return 0;
}
