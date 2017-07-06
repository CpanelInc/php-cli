/* ea-php-cli - include/path.h                  Copyright 2017 cPanel, Inc. */
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

#ifndef PATH_H
#define PATH_H

#include <unistd.h>

/* return the absolute basedir of a path */
char* path_basedir(char* buf, char* path, size_t size);

/* return the path of the php_cli.yaml file */
char* path_php_cli_yaml(char* buf, size_t size);

/* return the php version according to .htaccess files */
void path_get_htaccess_php_package(char* buf, size_t size,
                                   char* path, size_t path_size);

#endif /* PATH_H */
