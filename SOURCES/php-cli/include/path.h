#ifndef PATH_H
#define PATH_H

#include <unistd.h>

/* return the absolute basedir of a path */
char* path_basedir(char* buf, char* path, size_t size);

/* return the path of the php_cli.yaml file */
char* path_php_cli_yaml(char* buf, size_t size);

/* return the php version according to .htaccess files */
void path_get_htaccess_php_version(char* buf, size_t size,
                                   char* path, size_t path_size);

#endif /* PATH_H */
