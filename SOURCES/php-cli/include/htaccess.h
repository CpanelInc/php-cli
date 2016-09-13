#ifndef HTACCESS_H
#define HTACCESS_H

#include <unistd.h>

/* fetches the php version from an htaccess file */
void htaccess_get_php_version_from_file(char* buf, size_t size,
                                        char* path, size_t path_size);

#endif /* HTACCESS_H */
