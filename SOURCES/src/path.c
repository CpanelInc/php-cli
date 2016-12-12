/* ea-php-cli - src/path.c                      Copyright 2016 cPanel, Inc. */
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

#include "path.h"

#include <stdio.h>

#include <linux/limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "htaccess.h"
#include "strategy.h"

/* This function copies a null-terminated string containing the base
 * directory of "pathname" into buf, limited by size.
 *
 * If the input to pathname is null or a relative path, the basedir
 * will be the current working directory.
 *
 * On success, this function returns a pointer to buf.
 * On failure, this function returns NULL.
 */
char* path_basedir(char* buf, char* pathname, size_t size) {
   char* last_slash_index = 0;

   memset(buf, 0, size);
   /* null path returns cwd */
   if (pathname == 0) {
       getcwd(buf, size);
       strncat(buf, "/", 2);
       return buf;
   }
   last_slash_index = strrchr(pathname, '/');
   /* absolute path */
   if (pathname[0] == '/') {
       memcpy(buf, pathname, last_slash_index-pathname+1);
       return buf;
   }
   /* relative path */
   getcwd(buf, size);
   strncat(buf, "/", 2);
   if (last_slash_index) {
       strncat(buf, pathname, last_slash_index-pathname+1);
   }
   return buf;
}

char* path_php_cli_yaml(char* buf, size_t size) {
   if (size <= 0) {
       return 0;
   }
   char* exe_path = (char*)malloc(size);
   /* Couldn't allocate buffer, exiting */
   if (!exe_path) {
       return 0;
   } 
   memset(exe_path, 0, size);
   readlink("/proc/self/exe", exe_path, size);

   memset(buf, 0, size);
   path_basedir(buf, exe_path, size);
   strncat(buf, "php_cli.yaml", 13);

   free(exe_path);
   return buf;
}

void path_get_htaccess_php_version(char* buf, size_t size, char* path, size_t path_size) {
   char* basedir       = 0;
   char* htaccess_file = 0;

   memset(buf, 0, size);
   if (path == 0 || path[0] == 0) {
     return;
   }
   basedir = (char*)malloc(path_size);
   htaccess_file = (char*)malloc(path_size);
   path_basedir(basedir, path, path_size);

   strncpy(htaccess_file, basedir, path_size);
   strncat(htaccess_file, ".htaccess", 12);
   htaccess_get_php_version_from_file(buf, size, htaccess_file, path_size);

   while (strncmp(basedir, "/", 2) != 0 && strnlen(buf, size) == 0 ) {
     basedir[strnlen(basedir, path_size)-1] = 0; /* trim "/" */
     /* reuse htaccess_file as a temp buffer, it will be rebuilt */
     path_basedir(htaccess_file, basedir, path_size);
     strncpy(basedir, htaccess_file, path_size);

     strncpy(htaccess_file, basedir, strlen(path+1));
     strncat(htaccess_file, ".htaccess", 12);
     htaccess_get_php_version_from_file(buf, size, htaccess_file, path_size);
   }

   free(basedir);
   free(htaccess_file);
}
