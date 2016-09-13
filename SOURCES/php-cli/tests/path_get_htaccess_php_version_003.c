#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

#include "path.h"

int get_from_file_called = 0;

void __wrap_htaccess_get_php_version_from_file(char* buf, size_t size, char* path, size_t path_size) {
   get_from_file_called = 1;
   memset(buf, 0, size);
   if (strncmp(path, "/.htaccess", 16) == 0) {
     strncpy(buf, "70", 3);
   }
}

int main(int argc, char** argv) {
  char  testcase[1024] = "/test.php";
  char  version[8] = "junk";

  char  expected_version[8] = "70";

  printf("testing path_get_htaccess_php_version with ea_php70 in .htaccess in directory of \"%s\"\n", testcase);

  printf("  calling function path_get_htaccess_php_version(\"%s\", %d, \"%s\", %d)\n", version, 8, testcase, 1024);
  path_get_htaccess_php_version(version, 8, testcase, 1024);

  if (get_from_file_called == 0) {
    printf("ERROR: htaccess_get_php_version_from_file not called\n");
    return 1;
  } else {
    printf("  htaccess_get_php_version_from_file_called\n");
  }

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
