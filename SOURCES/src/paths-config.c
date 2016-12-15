/* ea-php-cli - src/paths-config.c              Copyright 2016 cPanel, Inc. */
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

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif /* HAVE_CONFIG_H */

#include "paths-config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "path.h"

int paths_config_read_paths_config(char* path, size_t size, struct paths_config* buf) {
    struct stat   path_stat;
    FILE*         fin;
    char          line[1024] = { 0 };

    char*         char_index;
    char          key[1024] = { 0 };
    char*         key_start;
    char*         key_end;
    char          value[1024] = { 0 };
    char*         value_start;
    char*         value_end;

    if (buf == 0) {
        return -1;
    }
    memset(buf, 0, sizeof(struct paths_config));
    if (path == 0 || path[0] == 0) {
        return 0;
    }
    if (stat(path, &path_stat) != 0 || S_ISREG(path_stat.st_mode) == 0) {
        return -1;
    }
    fin = fopen(path, "r");
    if (fin == NULL) {
        return -1;
    }
    while (fgets(line, sizeof(line), fin)) {
        memset(key, 0, 1024);
        if ((char_index = strchr(line, '\n')) != 0) { *char_index = 0; } /* trim newline */
        if ((char_index = strchr(line, '#')) != 0) { *char_index = 0; } /* trim comment */
        key_start = line;
        while (isspace(*key_start)) { key_start++; } /* trim leading key whitespace */
        if ((char_index = strchr(key_start, '=')) != 0) {
            key_end = char_index;
            while (isspace(*(key_end-1)) && key_end > key_start) { key_end--; }; /* trim trailing key whitespace */
            strncpy(key, key_start, key_end-key_start); /* capture trimmed key */
        }
        if (strnlen(key, 1024) > 1) {
            value_start = char_index+1;
            while (isspace(*value_start)) { value_start++; } /* trim leading value whitespace */
            value_end = value_start + strnlen(value_start, 1024);
            while (isspace(*(value_end-1)) && value_end > value_start) { value_end--; }
            strncpy(value, value_start, value_end-value_start); /* capture trimmed value */
        }
        if (strnlen(key, 1024) > 1 && strnlen(value, 1024) > 1) {
            if (strncmp(key, "dir_base", 20) == 0) { strncpy(buf->dir_base, value, PATHS_CONFIG_DIR_BASE_SIZE); }
            if (strncmp(key, "dir_logs", 20) == 0) { strncpy(buf->dir_logs, value, PATHS_CONFIG_DIR_LOGS_SIZE); }
            if (strncmp(key, "dir_domlogs", 20) == 0) { strncpy(buf->dir_domlogs, value, PATHS_CONFIG_DIR_DOMLOGS_SIZE); }
            if (strncmp(key, "dir_modules", 20) == 0) { strncpy(buf->dir_modules, value, PATHS_CONFIG_DIR_MODULES_SIZE); }
            if (strncmp(key, "dir_run", 20) == 0) { strncpy(buf->dir_run, value, PATHS_CONFIG_DIR_RUN_SIZE); }
            if (strncmp(key, "dir_conf", 20) == 0) { strncpy(buf->dir_conf, value, PATHS_CONFIG_DIR_CONF_SIZE); }
            if (strncmp(key, "dir_conf_includes", 20) == 0) { strncpy(buf->dir_conf_includes, value, PATHS_CONFIG_DIR_CONF_INCLUDES_SIZE); }
            if (strncmp(key, "dir_conf_userdata", 20) == 0) { strncpy(buf->dir_conf_userdata, value, PATHS_CONFIG_DIR_CONF_USERDATA_SIZE); }
            if (strncmp(key, "dir_docroot", 20) == 0) { strncpy(buf->dir_docroot, value, PATHS_CONFIG_DIR_DOCROOT_SIZE); }
            if (strncmp(key, "file_access_log", 20) == 0) { strncpy(buf->file_access_log, value, PATHS_CONFIG_FILE_ACCESS_LOG_SIZE); }
            if (strncmp(key, "file_error_log", 20) == 0) { strncpy(buf->file_error_log, value, PATHS_CONFIG_FILE_ERROR_LOG_SIZE); }
            if (strncmp(key, "file_conf", 20) == 0) { strncpy(buf->file_conf, value, PATHS_CONFIG_FILE_CONF_SIZE); }
            if (strncmp(key, "file_conf_mime_types", 25) == 0) { strncpy(buf->file_conf_mime_types, value, PATHS_CONFIG_FILE_CONF_MIME_TYPES_SIZE); }
            if (strncmp(key, "file_conf_srm_conf", 20) == 0) { strncpy(buf->file_conf_srm_conf, value, PATHS_CONFIG_FILE_CONF_SRM_CONF_SIZE); }
            if (strncmp(key, "file_conf_php_conf", 20) == 0) { strncpy(buf->file_conf_php_conf, value, PATHS_CONFIG_FILE_CONF_PHP_CONF_SIZE); }
            if (strncmp(key, "bin_httpd", 20) == 0) { strncpy(buf->bin_httpd, value, PATHS_CONFIG_BIN_HTTPD_SIZE); }
            if (strncmp(key, "bin_apachectl", 20) == 0) { strncpy(buf->bin_apachectl, value, PATHS_CONFIG_BIN_APACHECTL_SIZE); }
            if (strncmp(key, "bin_suexec", 20) == 0) { strncpy(buf->bin_suexec, value, PATHS_CONFIG_BIN_SUEXEC_SIZE); }
        }
    }
    fclose(fin);
    return 0;
}

void paths_config_load(struct paths_config* paths_config) {
   char default_location[] = "/etc/cpanel/ea4/paths.conf";
   paths_config_read_paths_config(default_location, sizeof(default_location), paths_config); 
}

