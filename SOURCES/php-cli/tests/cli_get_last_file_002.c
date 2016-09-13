#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "cli.h"

int main(int argc, char** argv) {
  char* testcase[] = { 0 };
  char  file[1024] = "unset";

  printf("testing cli_get_last_file on no element list\n");
  printf("  calling cli_get_last_file(\"%s\", %d, &{ 0 }\n", file, 1024);

  cli_get_last_file(file, 1024, testcase);

  if (strnlen(file, 1024) != 0) {
    printf("ERROR: file \"%s\" is not empty\n", file);
    return 1;
  } else {
    printf("  file \"%s\" is empty\n", file);
  }

  printf("test complete\n");
  return 0;
}
