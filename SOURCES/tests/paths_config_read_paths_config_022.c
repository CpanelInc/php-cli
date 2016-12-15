/* ea-php-cli - tests/paths_config_read_paths_config_022.c  Copyright 2016 cPanel, Inc. */
/*                                                     All rights Reserved. */
/* copyright@cpanel.net                                   http://cpanel.net */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "paths-config.h"

int xstat_called = 0;

int __wrap___xstat(int ver, const char* path, struct stat* buf) {
   xstat_called = 1;
   buf->st_mode = S_IFREG;
   return 0;
}

int fopen_called = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
   fopen_called = 1;
   return (FILE*)5;
}

int fgets_call_count = 0;

char* __wrap_fgets(char* line, size_t size, FILE* file) {
   printf("fgets, call %d\n", fgets_call_count);
   char* lines[] = { "bin_httpd=httpd\n" };
   if (fgets_call_count >= (int)(sizeof(lines)/sizeof(lines[0]))) {
       fgets_call_count++;
       return 0;
   }
   strncpy(line, lines[fgets_call_count], size);
   fgets_call_count++;
   return line;
}

int fclose_called = 0;

int __wrap_fclose(FILE* file) {
   fclose_called = 1;
   return 0;
}

int main(int argc, char** argv) {
  struct paths_config paths_config;
  char   file[1024] = "/a/regular/file.txt";
  int    return_code = 0;

  /* Assure garbage in paths_config */
  strncpy(paths_config.dir_base, "unset", 10);
  strncpy(paths_config.dir_logs, "unset", 10);
  strncpy(paths_config.dir_domlogs, "unset", 10);
  strncpy(paths_config.dir_modules, "unset", 10);
  strncpy(paths_config.dir_run, "unset", 10);
  strncpy(paths_config.dir_conf, "unset", 10);
  strncpy(paths_config.dir_conf_includes, "unset", 10);
  strncpy(paths_config.dir_conf_userdata, "unset", 10);
  strncpy(paths_config.dir_docroot, "unset", 10);
  strncpy(paths_config.file_access_log, "unset", 10);
  strncpy(paths_config.file_conf, "unset", 10);
  strncpy(paths_config.file_conf_mime_types, "unset", 10);
  strncpy(paths_config.file_conf_srm_conf, "unset", 10);
  strncpy(paths_config.bin_httpd, "unset", 10);
  strncpy(paths_config.bin_apachectl, "unset", 10);
  strncpy(paths_config.bin_suexec, "unset", 10);

  printf("testing paths_config_read_paths_config on regular file, no contents\n");
  printf("  calling paths_config_read_paths_config(\"%s\", %d, &paths_config\n", file, 1024);

  return_code = paths_config_read_paths_config(file, 1024, &paths_config);

  if (return_code != 0) {
    printf("ERROR: return code %d is not zero\n", return_code);
    return 1;
  } else {
    printf("  return code %d is zero\n", return_code);
  }

  if (xstat_called == 0) {
    printf("ERROR: no attempt to detect type of file\n");
    return 1;
  } else {
    printf("  attempt to detect type of file\n");
  }

  if (fopen_called == 0) {
    printf("ERROR: no attempt to open file\n");
    return 1;
  } else {
    printf("  fopen called\n");
  }

  if (fgets_call_count != 2) {
    printf("ERROR: fgets called %d times, not twice\n", fgets_call_count);
    return 1;
  } else {
    printf("  fgets called %d times, or twice\n", fgets_call_count);
  }

  if (fclose_called == 0) {
    printf("ERROR: no attempt to close file\n");
    return 1;
  } else {
    printf("  fclose called\n");
  }

  if (strnlen(paths_config.dir_base, 8) != 0) {
    printf("ERROR: paths_config.dir_base \"%s\" is not empty\n", paths_config.dir_base);
    return 1;
  } else {
    printf("  paths_config.dir_base \"%s\" is empty\n", paths_config.dir_base);
  }

  if (strnlen(paths_config.dir_logs, 8) != 0) {
    printf("ERROR: paths_config.dir_logs \"%s\" is not empty\n", paths_config.dir_logs);
    return 1;
  } else {
    printf("  paths_config.dir_logs \"%s\" is empty\n", paths_config.dir_logs);
  }

  if (strnlen(paths_config.dir_domlogs, 8) != 0) {
    printf("ERROR: paths_config.dir_domlogs \"%s\" is not empty\n", paths_config.dir_domlogs);
    return 1;
  } else {
    printf("  paths_config.dir_domlogs \"%s\" is empty\n", paths_config.dir_domlogs);
  }

  if (strnlen(paths_config.dir_modules, 8) != 0) {
    printf("ERROR: paths_config.dir_modules \"%s\" is not empty\n", paths_config.dir_modules);
    return 1;
  } else {
    printf("  paths_config.dir_modules \"%s\" is empty\n", paths_config.dir_modules);
  }

  if (strnlen(paths_config.dir_run, 8) != 0) {
    printf("ERROR: paths_config.dir_run \"%s\" is not empty\n", paths_config.dir_run);
    return 1;
  } else {
    printf("  paths_config.dir_run \"%s\" is empty\n", paths_config.dir_run);
  }

  if (strnlen(paths_config.dir_conf, 8) != 0) {
    printf("ERROR: paths_config.dir_conf \"%s\" is not empty\n", paths_config.dir_conf);
    return 1;
  } else {
    printf("  paths_config.dir_conf \"%s\" is empty\n", paths_config.dir_conf);
  }

  if (strnlen(paths_config.dir_conf_includes, 8) != 0) {
    printf("ERROR: paths_config.dir_conf_includes \"%s\" is not empty\n", paths_config.dir_conf_includes);
    return 1;
  } else {
    printf("  paths_config.dir_conf_includes \"%s\" is empty\n", paths_config.dir_conf_includes);
  }

  if (strnlen(paths_config.dir_conf_userdata, 8) != 0) {
    printf("ERROR: paths_config.dir_conf_userdata \"%s\" is not empty\n", paths_config.dir_conf_userdata);
    return 1;
  } else {
    printf("  paths_config.dir_conf_userdata \"%s\" is empty\n", paths_config.dir_conf_userdata);
  }

  if (strnlen(paths_config.dir_docroot, 8) != 0) {
    printf("ERROR: paths_config.dir_docroot \"%s\" is not empty\n", paths_config.dir_docroot);
    return 1;
  } else {
    printf("  paths_config.dir_docroot \"%s\" is empty\n", paths_config.dir_docroot);
  }

  if (strnlen(paths_config.file_access_log, 8) != 0) {
    printf("ERROR: paths_config.file_access_log \"%s\" is not empty\n", paths_config.file_access_log);
    return 1;
  } else {
    printf("  paths_config.file_access_log \"%s\" is empty\n", paths_config.file_access_log);
  }

  if (strnlen(paths_config.file_error_log, 8) != 0) {
    printf("ERROR: paths_config.file_error_log \"%s\" is not empty\n", paths_config.file_error_log);
    return 1;
  } else {
    printf("  paths_config.file_error_log \"%s\" is empty\n", paths_config.file_error_log);
  }

  if (strnlen(paths_config.file_conf, 8) != 0) {
    printf("ERROR: paths_config.file_conf \"%s\" is not empty\n", paths_config.file_conf);
    return 1;
  } else {
    printf("  paths_config.file_conf \"%s\" is empty\n", paths_config.file_conf);
  }

  if (strnlen(paths_config.file_conf_mime_types, 8) != 0) {
    printf("ERROR: paths_config.file_conf_mime_types \"%s\" is not empty\n", paths_config.file_conf_mime_types);
    return 1;
  } else {
    printf("  paths_config.file_conf_mime_types \"%s\" is empty\n", paths_config.file_conf_mime_types);
  }

  if (strnlen(paths_config.file_conf_srm_conf, 8) != 0) {
    printf("ERROR: paths_config.file_conf_srm_conf \"%s\" is not empty\n", paths_config.file_conf_srm_conf);
    return 1;
  } else {
    printf("  paths_config.file_conf_srm_conf \"%s\" is empty\n", paths_config.file_conf_srm_conf);
  }

  if (strnlen(paths_config.file_conf_php_conf, 8) != 0) {
    printf("ERROR: paths_config.file_conf_php_conf \"%s\" is not empty\n", paths_config.file_conf_php_conf);
    return 1;
  } else {
    printf("  paths_config.file_conf_php_conf \"%s\" is empty\n", paths_config.file_conf_php_conf);
  }

  if (strncmp(paths_config.bin_httpd, "httpd", 1024) != 0) {
    printf("ERROR: paths_config.bin_httpd \"%s\" is not \"httpd\"\n", paths_config.bin_httpd);
    return 1;
  } else {
    printf("  paths_config.bin_httpd \"%s\" is set correctly to \"httpd\"\n", paths_config.bin_httpd);
  }

  if (strnlen(paths_config.bin_apachectl, 8) != 0) {
    printf("ERROR: paths_config.bin_apachectl \"%s\" is not empty\n", paths_config.bin_apachectl);
    return 1;
  } else {
    printf("  paths_config.bin_apachectl \"%s\" is empty\n", paths_config.bin_apachectl);
  }

  if (strnlen(paths_config.bin_suexec, 8) != 0) {
    printf("ERROR: paths_config.bin_suexec \"%s\" is not empty\n", paths_config.bin_suexec);
    return 1;
  } else {
    printf("  paths_config.bin_suexec \"%s\" is empty\n", paths_config.bin_suexec);
  }

  printf("test complete\n");
  return 0;
}
