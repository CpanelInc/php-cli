/* ea-php-cli - src/htaccess.c                  Copyright 2016 cPanel, Inc. */
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

#include "htaccess.h"

/* fetches the php version from an htaccess file */
void htaccess_get_php_version_from_file(char* buf, size_t size, char* path, size_t path_size) {
   struct stat path_stat;
   FILE*       fin;
   char        line[1024];
   char*       position;
   char*       add_type_line = "AddType application/x-httpd-ea-php";

   memset(buf, 0, size);
   if (path == 0 || path[0] == 0 ) {
     return;
   }
   /* check if not a file */
   if (stat(path, &path_stat) != 0 || S_ISREG(path_stat.st_mode) == 0) {
     return;
   } 
   fin = fopen(path, "r");
   if (fin) {
       while  (buf[0] == 0 && fgets(line, 1024, fin) != NULL) {
           if ((position = strstr(line, add_type_line)) != 0) {
               strncpy(buf, position+strlen(add_type_line), 2);
           }
       }
       fclose(fin);
   }
}


