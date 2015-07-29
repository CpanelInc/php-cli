#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <yaml.h>

#include "php-cli.h"

key_pair_t      *php_cli_head = NULL;
key_pair_t      *php_config_head = NULL;
key_pair_t      *paths_conf_head = NULL;

char            cwd [1024];
char            *myexepath;
char            myexe [1024];
char            *testing_root_dir = NULL;
char            *ea_php_conf_yaml = NULL;
char            *php_bin_pattern = NULL;

/* TODO when this is put into the main repo:
 *   Add a comment in Cpanel::Config::Apache that if its logic changes to be sure
 *   to update <WHEREVER THIS LIVES IN /usr/local/cpanel HERE> accordingly also.
 */

char            apache_conf_dir [1024] = "/etc/apache2/conf.d";

int file_exists (char *fname)
{
    if( access( fname, F_OK ) != -1 )
        return 1;

    return 0;
}

char *get_file_path (char *fname)
{
    int     i, len;
    char    *path;

    len = strlen (fname);
    for (i = len - 1; i >= 0; --i)
    {
        if (fname [i] == '/')
        {
            if (i == 0)
            {
                return get_string_copy (cwd);
            }
            else
            {
                path = get_string_copy (fname);
                path [i] = 0;

                return path;
            }
        }
    }
   
    return get_string_copy (cwd); 
}

char *get_file_path_search (char *fname)
{
    int     i, len;
    char    *path;

    len = strlen (fname);
    for (i = len - 1; i >= 0; --i)
    {
        if (fname [i] == '/')
        {
            if (i == 0)
            {
                path = get_string_copy ("/");
                return path;
            }

            path = get_string_copy (fname);
            path [i] = 0;

            return path;
        }
    }
   
    return get_string_copy (cwd); 
}

/* to make this faster, we are going to only read data when and if we need it
 * rather then load the configs everytime.
 */

int            b_loaded_php_cli_yaml = 0;
int            b_loaded_paths_conf = 0;

void load_paths_conf ()
{
    char    *paths_conf = NULL;

    if (b_loaded_paths_conf)
        return;

    b_loaded_paths_conf = 1;

/* TODO when this is put into the main repo:
 *   Add a comment in Cpanel::Config::Apache that if its logic changes to be sure
 *   to update <WHEREVER THIS LIVES IN /usr/local/cpanel HERE> accordingly also.
 */

    paths_conf = get_string_copy ("/etc/cpanel/ea4/paths.conf");
    if (file_exists (paths_conf))
    {
        char    *ptr;

        paths_conf_head = parse_conf_file (paths_conf);
        ptr = get_value (paths_conf_head, "dir_conf");
        if (ptr != NULL)
            strcpy (apache_conf_dir, ptr);
    }

    return;
}

void load_php_cli ()
{
    char    myconfig_path [1024];

    if (b_loaded_php_cli_yaml)
        return;

    b_loaded_php_cli_yaml = 1;

    readlink ("/proc/self/exe", myexe, sizeof (myexe));
    
    myexepath = get_file_path (myexe);

    //printf ("CWD :%s:\n", cwd); 
    //printf ("MyPath :%s:\n", myexepath); 

    strcpy (myconfig_path, myexepath);
    strcat (myconfig_path, "/");
    strcat (myconfig_path, "php_cli.yaml");
 
    //printf ("myconfig_path :%s:\n", myconfig_path);
 
    if (file_exists (myconfig_path))
    { 
        php_cli_head = parse_yaml_file (myconfig_path);
        //debug_print_pairs (php_cli_head);

        ea_php_conf_yaml = get_value (php_cli_head, "ea_php_yaml");
        testing_root_dir = get_value (php_cli_head, "testing_root_dir");
    }
    else
    {
        //printf ("No php_cli.yaml exists\n");
    }

    if (ea_php_conf_yaml == NULL)
    {
        load_paths_conf ();
        ea_php_conf_yaml = get_concat_strings (apache_conf_dir, "/php.conf.yaml");
    }
        
    php_bin_pattern = get_value (php_cli_head, "php_bin_pattern");
    if (php_bin_pattern == NULL)
        php_bin_pattern = get_string_copy ("/opt/cpanel/ea-php%s/root/usr/bin/php");

    return;
}

key_pair_t*     htaccess_search_path_ll (char *path)
{
    key_pair_t      *head = NULL;
    key_pair_t      *last = NULL;
    char            *working_path;
    int             count = 0;

    load_php_cli ();

    working_path = get_string_copy (path);

    last = alloc_key_pair ();
    last->key = get_string_copy ("place_holder"); 
    last->value = get_string_copy (working_path);

    head = last;

    while (1)
    {
        if (testing_root_dir != NULL && !strcmp (testing_root_dir, working_path))
            break;

        if (!strcmp (working_path, "/"))
            break;

        last->next = alloc_key_pair ();
        last = last->next;

        last->key = get_string_copy ("place_holder"); 
        working_path = get_file_path_search (working_path);

        last->value = working_path;

        count++;
        if (count > 50) break;
    } 

    return head;
}

void print_search_list (key_pair_t *next)
{
    while (next != NULL)
    {
        printf ("%s\n", next->value);
        next = next->next;
    }

    return;
}

char*   get_default_php_version ()
{
    char    *php_version_string = NULL;
    int     len;

    load_php_cli ();
    if (!file_exists (ea_php_conf_yaml))
    {
        fputs ("Easy Apache PHP Config file not found\n", stderr);
        exit (1);
    }

    php_config_head = parse_yaml_file (ea_php_conf_yaml);
    php_version_string = get_value (php_config_head, "phpversion");

    /* last 2 chars are the version number */

    if (php_version_string == NULL)
    {
        fputs ("Default PHP not found\n", stderr);
        exit (1);
    }
  
    len = strlen (php_version_string); 
    return &php_version_string [len - 2];
}

char*   create_path (char *path, char *fname)
{
    char    static_out [1024];

    static_out [0] = 0;
    strcpy (static_out, path);
    if (strcmp (path, "/"))
        strcat (static_out, "/");
    strcat (static_out, fname);

    return get_string_copy (static_out);
}

static char searchy [] = "AddType application/x-httpd-";

char*   search_for_php_version (key_pair_t *next)
{
    char    *htaccess_path;
    char    buffer [1024];

    while (next != NULL)
    {
        htaccess_path = create_path (next->value, ".htaccess");
        if (file_exists (htaccess_path))
        {
            FILE    *fh;
            int     idx;

            fh = fopen (htaccess_path, "r");
            if (fh != NULL)
            {
                while (fgets (buffer, sizeof (buffer), fh) != NULL)
                {
                    idx = istrstr (buffer, searchy);
                    if (idx >= 0)
                    {
                        int     len = strlen (searchy);
                        char    php_version [1024];

                        strcpy (php_version, &buffer [idx + len]);
                        php_version [2] = 0;

                        fclose (fh);

                        return get_string_copy (php_version);
                    }
                }

                fclose (fh);
            }
        }

        next = next->next;
    }

    return get_default_php_version ();
}

int main (
    int     argc,
    char    *argv [])
{
    int     i;
    int     b_found_php_file = 0;
    char    *php_file_path;
    key_pair_t  *htaccess_search_list = NULL;
    char    *php_version;
    char    php_bin [1024];

    key_pair_t  *command_line = NULL;
    int     xargc;
    char    **xargv;

    getcwd (cwd, sizeof (cwd));

    /* was the version passed in on the command line
     *
     * i.e.  -ea_php 54
     *
     */

    for (i = 1; i < argc; ++i)
    {
        if (!strcmp (argv [i], "-ea_php"))
        {
            if ((i + 1) < argc)
            {
                php_version = get_string_copy (argv [i + 1]);
                goto SKIP;
            }
        }
    }

    /* now let's find the php file and see if it has a path */

    for (i = 1, b_found_php_file = 0; i < argc; ++i)
    {
        if (file_exists (argv [i]))
        {
            php_file_path = get_file_path (argv [i]);
            b_found_php_file = 1;

            //printf ("PATH (%s)\n", php_file_path);
        }
    }

    if (!b_found_php_file)
    {
        php_file_path = get_string_copy (cwd);
        //printf("PATH (%s)\n", cwd);
    }

    htaccess_search_list = htaccess_search_path_ll (php_file_path);
    //print_search_list (htaccess_search_list);

    //printf ("Default Version :%s:\n", get_default_php_version ());

    php_version = search_for_php_version (htaccess_search_list);

SKIP:

    load_php_cli ();
    sprintf (php_bin, php_bin_pattern, php_version);
    
    if (!file_exists (php_bin))
    {
        fprintf (stderr, "Chosen php (%s) does not exist\n", php_version);
        php_version = get_default_php_version ();
        sprintf (php_bin, php_bin_pattern, php_version);
    }
 
    //printf ("PHP Version (%s) EXT (%s) EXISTS (%d)\n", php_version, php_bin, file_exists (php_bin)); 

    /* now process command line parameters */

    {
        key_pair_t      *next = NULL;
        key_pair_t      *current = NULL;

        xargc = 0;
        for (i = 0; i < argc; ++i)
        {
            if (!strcmp (argv [i], "-ea_php"))
            {
                if ((i + 1) < argc)
                {
                    i++; /* double increment */
                    continue;
                }
            }

            current = alloc_key_pair ();
            current->key = get_string_copy ("place_holder");

            if (i == 0)
                current->value = get_string_copy (&php_bin [0]);
            else
                current->value = get_string_copy (argv [i]);

            xargc ++;

            if (command_line == NULL)
            {
                command_line = current;
            }
            else
            {
                next->next = current;
            }

            next = current;
        }

        xargv = malloc ((xargc + 1) * sizeof (char*));
        for (i = 0, next = command_line; i < xargc; ++i)
        {
            xargv [i] = get_string_copy (next->value);
            next = next->next;
        }

        xargv [i] = NULL;
    }

#if 0
    printf ("Command Line Num (%d) :%s:\n", xargc, php_bin);
    for (i = 0; i < xargc; ++i)
    {
        printf ("    :%s:\n", xargv [i]);
    }
#endif

    i = execv (&php_bin [0], xargv);

    printf ("php_cli unable to execute php :%d: (%d) :%s:\n", i, errno, strerror (errno));

    return (0);
}

