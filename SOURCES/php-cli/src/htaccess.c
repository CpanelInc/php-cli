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


