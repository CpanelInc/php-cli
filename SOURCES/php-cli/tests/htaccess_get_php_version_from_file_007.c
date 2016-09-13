#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "htaccess.h"

/* act like the path exists, and is a file */
int __wrap___xstat(int ver, const char* path, struct stat* buf) {
    buf->st_mode = S_IFREG;
    return 0;
}

int fopen_called = 0;

/* act like the file can be opened */
FILE* __wrap_fopen(const char* path, const char* mode) {
    fopen_called = 1;
    return (FILE*)5;
}

int fgets_call_count = 0;

/* but the file is empty */
char* __wrap_fgets(char* buf, int size, FILE* file) {
    fgets_call_count++;
    return 0;
}

int fclose_called = 0;

int __wrap_fclose(FILE* file) {
    fclose_called = 1;
    return 0;
}

int main(int argc, char** argv) {
  char testcase[1024] = "/some/path/.htaccess";
  char version[8] = "junk";

  char expected_fgets_call_count = 1;

  printf("testing htaccess_get_php_version_from_file on empty regular file\n");
  printf("  calling htaccess_get_php_version_from_file(\"%s\", %d, \"%s\", %d)\n",
         version, 8, testcase, 1024);
  htaccess_get_php_version_from_file(version, 8, testcase, 1024);

  if (fopen_called == 0) {
    printf("ERROR: no attempt to open file\n");
    return 1;
  } else {
    printf("  fopen called\n");
  }

  if (fgets_call_count != expected_fgets_call_count) {
    printf("ERROR: fgets called %d times, expected %d\n", fgets_call_count, expected_fgets_call_count);
    return 1;
  } else {
    printf("  fgets call count is correct\n");
  }

  if (fclose_called == 0) {                                              
    printf("ERROR: no attempt to close file\n");                         
    return 1;                                                           
  } else {                                                              
    printf("  fclose called\n");                                         
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
