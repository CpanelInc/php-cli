/* ea-php-cli - src/htaccess.c                  Copyright 2017 cPanel, Inc. */
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
void htaccess_get_php_package_from_file(char* buf, size_t size, char* path, size_t path_size) {
   struct stat path_stat;
   FILE*       fin;
   char        line[1024];
   char        line_copy[1027];
   char*       position;
   char*       add_type_line = "AddType application/x-httpd-";

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

           // strip comments, have to prepend a space or the strtok() hack won't work for lines beginning w/ #
           sprintf(line_copy, " %s\n", line);
           strtok(line_copy, "#");

           if ((position = strstr(line_copy, add_type_line)) != 0) {

               strncpy(buf, position+strlen(add_type_line), size); // grab the package an everything after it up to `size` (i.e. 20)
               strtok(buf, " "); // strip of the space and everything after it
               strtok(buf, "___"); // Look for '3 underscores' at https://cpanel.wiki/display/EA/Adding+a+PHP+Handler for more details

               // buf is our package but is it a PHP package
               if (strstr(buf, "-php") == NULL) { // ?TODO/YANGI?: make this even smarter? e.g. =~ m/-php\d\d$/
                    buf[0] = 0;
               }
           }
       }
       fclose(fin);
   }
}


