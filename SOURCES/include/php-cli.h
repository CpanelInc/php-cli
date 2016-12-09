/* ea-php-cli - include/php-cli.h               Copyright 2016 cPanel, Inc. */
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
