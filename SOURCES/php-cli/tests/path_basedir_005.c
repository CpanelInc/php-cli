#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

#include "path.h"

/* mock of char* getcwd(char* buf, size_t size) */
char* __wrap_getcwd(char* buf, size_t size) {
    char* return_value = 0;
    if (size > strlen("/home/edwbuck")) {
        /* getcwd returns with no trailing slash */
        return_value = strncpy(buf, "/home/edwbuck", size);
        if (return_value) {
            buf[size-1] = 0;
        }
    }
    return return_value;
}

int main(int argc, char** argv) {
  char* testcase = "my/subdir/bob.php";
  char  basedir[PATH_MAX] = { 0 };
  char* return_value = 0;
  char* expected_basedir = "/home/edwbuck/my/subdir/";

  printf("testing path_basedir on path \"my/subdir/bob.php\"\n");

  printf("  calling function path_basedir()\n");
  return_value = path_basedir(basedir, testcase, PATH_MAX);

  if (return_value == 0) {
    printf("ERROR: return value %p is 0\n", return_value);
    return 1;
  }
  if (return_value != basedir) {
    printf("ERROR: return value %p is not %p\n", return_value, basedir);
    return 1;
  }
  if (strncmp(basedir, expected_basedir, 20) != 0) {
    printf("ERROR: basedir %s is not %s\n", basedir, expected_basedir);
    return 1;
  }

  printf("test complete\n");
  return 0;
}
