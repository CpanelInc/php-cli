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

