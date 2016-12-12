/* ea-php-cli - tests/parse_yaml.c              Copyright 2016 cPanel, Inc. */
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

#include <unistd.h>
#include <stdio.h>

#include "php-cli.h"

int main (
    int     argc,
    char    *argv [])
{
    char    *old = "/etc/apache2/conf.d/php.conf.yaml"; // 11.52
    char    *new = "/etc/cpanel/ea4/php.conf"; // 11.54+
    FILE    *fh;

    key_pair_t      *my_parse = NULL;

    if (argc > 1)
        new = argv [1];

    my_parse = parse_yaml_file (new);
    if ( !my_parse )
        parse_yaml_file (old);

    debug_print_pairs (my_parse);

    return 0;
}

