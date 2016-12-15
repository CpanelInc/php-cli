/* ea-php-cli - include/paths-config.h          Copyright 2016 cPanel, Inc. */
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

#ifndef PATHS_CONFIG_H
#define PATHS_CONFIG_H

#include <unistd.h>

#define PATHS_CONFIG_DIR_BASE_SIZE 256
#define PATHS_CONFIG_DIR_LOGS_SIZE 256
#define PATHS_CONFIG_DIR_DOMLOGS_SIZE 256
#define PATHS_CONFIG_DIR_MODULES_SIZE 256
#define PATHS_CONFIG_DIR_RUN_SIZE 256
#define PATHS_CONFIG_DIR_CONF_SIZE 256
#define PATHS_CONFIG_DIR_CONF_INCLUDES_SIZE 256
#define PATHS_CONFIG_DIR_CONF_USERDATA_SIZE 256
#define PATHS_CONFIG_DIR_DOCROOT_SIZE 256
#define PATHS_CONFIG_FILE_ACCESS_LOG_SIZE 256
#define PATHS_CONFIG_FILE_ERROR_LOG_SIZE 256
#define PATHS_CONFIG_FILE_CONF_SIZE 256
#define PATHS_CONFIG_FILE_CONF_MIME_TYPES_SIZE 256
#define PATHS_CONFIG_FILE_CONF_SRM_CONF_SIZE 256
#define PATHS_CONFIG_FILE_CONF_PHP_CONF_SIZE 256
#define PATHS_CONFIG_BIN_HTTPD_SIZE 256
#define PATHS_CONFIG_BIN_APACHECTL_SIZE 256
#define PATHS_CONFIG_BIN_SUEXEC_SIZE 256

struct paths_config {
  char dir_base[PATHS_CONFIG_DIR_BASE_SIZE];
  char dir_logs[PATHS_CONFIG_DIR_LOGS_SIZE];
  char dir_domlogs[PATHS_CONFIG_DIR_DOMLOGS_SIZE];
  char dir_modules[PATHS_CONFIG_DIR_MODULES_SIZE];
  char dir_run[PATHS_CONFIG_DIR_RUN_SIZE];
  char dir_conf[PATHS_CONFIG_DIR_CONF_SIZE];
  char dir_conf_includes[PATHS_CONFIG_DIR_CONF_INCLUDES_SIZE];
  char dir_conf_userdata[PATHS_CONFIG_DIR_CONF_USERDATA_SIZE];
  char dir_docroot[PATHS_CONFIG_DIR_DOCROOT_SIZE];
  char file_access_log[PATHS_CONFIG_FILE_ACCESS_LOG_SIZE];
  char file_error_log[PATHS_CONFIG_FILE_ERROR_LOG_SIZE];
  char file_conf[PATHS_CONFIG_FILE_CONF_SIZE];
  char file_conf_mime_types[PATHS_CONFIG_FILE_CONF_MIME_TYPES_SIZE];
  char file_conf_srm_conf[PATHS_CONFIG_FILE_CONF_SRM_CONF_SIZE];
  char file_conf_php_conf[PATHS_CONFIG_FILE_CONF_PHP_CONF_SIZE];
  char bin_httpd[PATHS_CONFIG_BIN_HTTPD_SIZE];
  char bin_apachectl[PATHS_CONFIG_BIN_APACHECTL_SIZE];
  char bin_suexec[PATHS_CONFIG_BIN_SUEXEC_SIZE];
};

int  paths_config_read_paths_config(char* path, size_t size, struct paths_config* buf);
void paths_config_load(struct paths_config* buf);

#endif /* PATHS_CONFIG_H */
