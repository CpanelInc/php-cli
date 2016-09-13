#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "cli.h"

int main(int argc, char** argv) {
  char* testcase[] = { "-ea_php", "54", 0 };
  char version[8] = "junk";

  char* expected_version = "54";

  printf("testing cli_get_php_version on \"-ea_php 54\"\n");
  printf("  calling cli_get_php_version(\"%s\", %d, @{\"-ea_php\", \"54\", 0})\n",
         version, 8);

  cli_get_php_version(version, 8, testcase);

  if (strnlen(version, 8) == 0 || 
      strncmp(expected_version, version, 8) != 0) {
    printf("ERROR: version \"%s\" is not \"%s\"\n", 
           version, expected_version);
    return 1;
  } else {
    printf("  version \"%s\" is \"%s\"\n", 
           version, expected_version);
  }

  printf("test complete\n");
  return 0;
}
