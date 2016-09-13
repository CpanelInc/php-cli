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

int     istrstr (char *haystack, char *needle)
{
    char    uhaystack [1024];
    char    uneedle [1024];

    int     i, len, nlen;

    len = strlen (needle);
    nlen = len;
    for (i = 0; i < len; ++i)
    {
        uneedle [i] = toupper (needle [i]);
    }

    uneedle [i] = 0;
    
    len = strlen (haystack);
    for (i = 0; i < len; ++i)
    {
        uhaystack [i] = toupper (haystack [i]);
    }

    uhaystack [i] = 0;

    for (i = 0; i < len; ++i)
    {
        if (!strncmp (uneedle, &uhaystack [i], nlen))
        {
            return i;
        }
    }

    return -1;
}

char* get_string_copy (char* in)
{
    int     len = strlen (in);
    char    *out = malloc (len + 1);
    strcpy (out, in);

    return out;
}

char* get_concat_strings (char* a, char* b)
{
    int     len = strlen (a) + strlen (b);
    char    *out = malloc (len + 1);

    strcpy (out, a);
    strcat (out, b);

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

key_pair_t*     parse_conf_file (char  *fname)
{
    FILE            *fh;
    key_pair_t      *head = NULL;  
    key_pair_t      *last = NULL;

    char            buffer [1024];
    char            key [1024];
    char            value [1024];

    int             i, len;

    fh = fopen (fname, "r");
    if (fh == NULL)
        return NULL;

    while (fgets (buffer, sizeof (buffer), fh))
    {
        /* chomp */
        len = strlen (buffer);
        if (buffer [len - 1] == '\n')
            buffer [len - 1] = 0;

        if (buffer [0] == '#')
            continue;

        /* search for = sign */

        i = istrstr (buffer, "=");
        if (i < 0)
            continue;

        strcpy (key, buffer);
        key [i] = 0;

        strcpy (value, &buffer [i + 1]);

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

        last->key = get_string_copy (key);
        last->value = get_string_copy (value);
    }

    fclose (fh);

    return head;
}

