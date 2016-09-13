#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "htaccess.h"

/* act like the path exists, and is a regular file */
int __wrap___xstat(int ver, const char* path, struct stat* buf) {
    buf->st_mode = S_IFREG;
    return 0;
}

int fopen_called = 0;

/* but the file cannot be opened */
FILE* __wrap_fopen(const char* path, const char* mode) {
    fopen_called = 1;
    return (FILE*)0;
}

int unauthorized_call_fgets = 0;

char* __wrap_fgets(char* buf, int size, FILE* file) {
    unauthorized_call_fgets = 1;
    return 0;
}

int unauthorized_call_fclose = 0;

int __wrap_fclose(FILE* file) {
    unauthorized_call_fclose = 1;
    return 0;
}

int main(int argc, char** argv) {
  char testcase[1024] = "/some/path/.htaccess";
  char version[8] = "junk";

  printf("testing htaccess_get_php_version_from_file on regular file that cannot be opened\n");
  printf("  calling htaccess_get_php_version_from_file(\"%s\", %d, \"%s\", %d)\n",
         version, 8, testcase, 1024);
  htaccess_get_php_version_from_file(version, 8, testcase, 1024);

  if (fopen_called == 0) {
    printf("ERROR: no attempt to open file\n");
  } else {
    printf("  fopen called\n");
  }

  if (unauthorized_call_fgets) {
    printf("ERROR: attempt to read from directory occurred\n");
    return 1;
  } else {
    printf("  fgets not called\n");
  }

  if (unauthorized_call_fclose) {
    printf("ERROR: attempt to close file occurred\n");                         
    return 1;                                                           
  } else {                                                              
    printf("  fclose not called\n");                                         
  }

  if (strnlen(version, 8) != 0) {
    printf("ERROR: version %s is not empty\n", version);
    return 1;
  } else {
    printf("  version \"%s\" is empty\n", version);
  }

  printf("test complete\n");
  return 0;
}
