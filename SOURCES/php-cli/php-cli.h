#ifndef       __PHP_CLI_H_
#define       __PHP_CLI_H_

typedef struct key_pair_ll_s key_pair_t;

struct key_pair_ll_s
{
    char        *key;
    char        *value;

    key_pair_t  *next;
};

extern key_pair_t*     parse_yaml_file (char  *fname);
extern key_pair_t*     parse_conf_file (char  *fname);
extern void debug_print_pairs (key_pair_t *next);
extern char* get_value (key_pair_t *next, char* key);
extern char* get_string_copy (char* in);
extern char* get_concat_strings (char* a, char* b);
extern key_pair_t*     alloc_key_pair ();
extern int     istrstr (char *haystack, char *needle);

#endif    /*  __PHP_CLI_H_ */

