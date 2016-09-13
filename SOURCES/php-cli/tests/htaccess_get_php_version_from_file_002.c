#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "htaccess.h"

int unauthorized_call__xstat = 0;

int __wrap___xstat(int ver, const char* path, struct stat* buf) {
   unauthorized_call__xstat = 1;
   return -1;
}

int unauthorized_call_fopen = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
  unauthorized_call_fopen = 1;
  return 0;
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
  char testcase[1024] = "";
  char version[8] = "junk";

  printf("testing htaccess_get_php_version_from_file on empty string\n");
  printf("  calling htaccess_get_php_version_from_file(\"%s\", %d, \"%s\", %d)\n", version, 8, testcase, 1024);
  htaccess_get_php_version_from_file(version, 8, testcase, 1024);

  if (unauthorized_call__xstat) {
    printf("ERROR: attempt to lookup path occurred\n");
    return 1;
  } else {
    printf("  stat not called\n");
  }

  if (unauthorized_call_fopen) {
    printf("ERROR: attempt to open directory occurred\n");
    return 1;
  } else {
    printf("  fopen not called\n");
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
