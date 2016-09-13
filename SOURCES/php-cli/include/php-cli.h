#ifndef PHP_CLI_H
#define PHP_CLI_H

typedef struct yaml_s yaml_struct;

typedef struct key_pair_ll_s key_pair_t;



struct key_pair_ll_s
{
    char        *key;
    char        *value;
    key_pair_t  *next;
};

key_pair_t* parse_yaml_file(char  *fname);
key_pair_t* parse_conf_file(char  *fname);
void        debug_print_pairs(key_pair_t* next);
char*       get_value(key_pair_t *next, char* key);
char*       get_string_copy(char* in);
char*       get_concat_strings(char* a, char* b);
key_pair_t* alloc_key_pair();
int         istrstr(char* haystack, char* needle);


#endif /* PHP_CLI_H */

