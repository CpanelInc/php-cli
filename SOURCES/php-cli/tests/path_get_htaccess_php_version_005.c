#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>

#include "path.h"

int get_from_file_call_count = 0;

void __wrap_htaccess_get_php_version_from_file(char* buf, size_t size, char* path, size_t path_size) {
   get_from_file_call_count++;
   memset(buf, 0, size);
}

int main(int argc, char** argv) {
  char  testcase[1024] = "/a/deep/test/test.php";
  char  version[8] = "junk";

  int   expected_get_from_file_call_count = 4;

  printf("testing path_get_htaccess_php_version with no php version in .htaccess path for \"%s\"\n", testcase);

  printf("  calling function path_get_htaccess_php_version(\"%s\", %d, \"%s\", %d)\n", version, 8, testcase, 1024);
  path_get_htaccess_php_version(version, 8, testcase, 1024);

  if (get_from_file_call_count != expected_get_from_file_call_count) {
    printf("ERROR: htaccess_get_php_version_from_file called %d times, expected %d\n",
           get_from_file_call_count, expected_get_from_file_call_count);
    return 1;
  } else {
    printf("  htaccess_get_php_version_from_file call count is correct\n");
  }

  if (strnlen(version, 8) != 0) {
    printf("ERROR: version \"%s\" is not empty\n", version);
    return 1;
  } else {
    printf("  version \"%s\" is empty\n", version);
  }

  printf("test complete\n");
  return 0;
}
