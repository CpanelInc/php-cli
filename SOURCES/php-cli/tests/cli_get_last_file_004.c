#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"

int executable_checked = 0;

/* mock of int access(const char* pathname, int flags) */
int __wrap_access(const char* pathname, int flags) {
    if (strcmp("executable", pathname) == 0) {
       executable_checked = 1;
    }
    return strcmp("third", pathname) ? -1 : 0;
}

int main(int argc, char** argv) {
  char* testcase[] = { "executable", "first", "second", "third", "fourth" };
  char  file[1024] = "unset";

  char* expected_file = "third";

  printf("testing cli_get_last_file on \"first second third fourth\" where \"third\" is a file\n");

  printf("  calling cli_get_last_file(\"%s\", %d, &{ \"first\", \"second\", \"third\", \"fourth\"})\n", file, 1024);
  cli_get_last_file(file, 1024, testcase);

  if (executable_checked) {
    printf("ERROR: executable was checked as a last file\n");
    return 1;
  } else {
    printf("  executable was not checked as a last file\n");
  }

  if (strnlen(file, 1024) == 0 ||
      strncmp(expected_file, file, 1024) != 0) {
    printf("ERROR: file \"%s\" is not \"%s\"\n", file, expected_file);
    return 1;
  } else {
    printf("  file \"%s\" is \"%s\"\n", file, expected_file);
  }

  printf("test complete\n");
  return 0;
}
