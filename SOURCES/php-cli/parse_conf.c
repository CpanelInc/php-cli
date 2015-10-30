#include <unistd.h>
#include <stdio.h>

#include "php-cli.h"

int main (
    int     argc,
    char    *argv [])
{
    char    *fname = "/etc/cpanel/ea4/paths.conf";
    FILE    *fh;

    key_pair_t      *my_parse = NULL;

    if (argc > 1)
        fname = argv [1];

    my_parse = parse_conf_file (fname);
    debug_print_pairs (my_parse);


    return 0;
}

