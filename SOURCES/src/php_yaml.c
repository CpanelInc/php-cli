/* ea-php-cli - src/php_yaml.c                  Copyright 2016 cPanel, Inc. */
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

#include <yaml.h>

#include "php-cli.h"

char* get_value (key_pair_t *next, char* key)
{
    while (next != NULL)
    {
        if (!strcmp (next->key, key))
            return next->value;

        next = next->next;
    } 

    return NULL;
}

char* get_string_copy (char* in)
{
    int     len = strlen (in);
    char    *out = malloc (len + 1);
    strcpy (out, in);

    return out;
}

key_pair_t*     alloc_key_pair ()
{
    key_pair_t  *pair;

    pair = malloc (sizeof (key_pair_t));
    memset ((void*)pair, 0, sizeof (key_pair_t));

    return pair;
}

key_pair_t*     parse_yaml_file(char  *fname) {
    yaml_parser_t   parser;
    FILE            *fh;
    yaml_token_t    token;
    key_pair_t      *head = NULL;  
    key_pair_t      *last = NULL;

    int             token_type = 0; /* 0 unused, 1 key, 2 value */

    if (!yaml_parser_initialize (&parser))
    {
        fputs("Failed to initialize yaml parser!\n", stderr);
        exit (1);
    } 

    fh = fopen (fname, "r");
    if (fh == NULL)
        return NULL;

    yaml_parser_set_input_file(&parser, fh);

    do {
        yaml_parser_scan(&parser, &token);

        switch(token.type)
        {
        /* Stream start/end */
        case YAML_STREAM_START_TOKEN: break;
        case YAML_STREAM_END_TOKEN:   break;

        /* Token types (read before actual token) */
        case YAML_KEY_TOKEN:
            if (last != NULL)
            {
                last->next = alloc_key_pair ();
                last = last->next;
            }
            else
            {
                last = alloc_key_pair ();
            }

            if (head == NULL)
                head = last;

            token_type = 1;
            break;
        case YAML_VALUE_TOKEN:
            token_type = 2;
            break;

        /* Block delimeters */
        case YAML_BLOCK_SEQUENCE_START_TOKEN: break;
        case YAML_BLOCK_ENTRY_TOKEN:          break;
        case YAML_BLOCK_END_TOKEN:            break;
        /* Data */
        case YAML_BLOCK_MAPPING_START_TOKEN:  break;
        case YAML_SCALAR_TOKEN:
            if (token_type == 1)
                last->key = get_string_copy (token.data.scalar.value);
            else if (token_type == 2)
                last->value = get_string_copy (token.data.scalar.value);
            break;
        }

        if(token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while(token.type != YAML_STREAM_END_TOKEN);

    yaml_token_delete(&token);
    yaml_parser_delete (&parser);

    return head;
}

void debug_print_pairs (key_pair_t *next)
{
    while (next != NULL)
    {
        printf ("KEY (%s) VALUE (%s)\n", next->key, next->value);
        next = next->next;
    } 

    return;
}

