#include <unistd.h>
#include <stdio.h>

#include "php_cli.h"

int main (
    int     argc,
    char    *argv [])
{
    char    *fname = "/etc/apache2/conf.d/php.conf.yaml";
    FILE    *fh;

    key_pair_t      *my_parse = NULL;

    if (argc > 1)
        fname = argv [1];

    my_parse = parse_yaml_file (fname);
    debug_print_pairs (my_parse);

    return 0;
}

