/* ea-php-cli - src/cli-config.c                Copyright 2016 cPanel, Inc. */
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

#include "cli-config.h"

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>

#include "path.h"

int cli_config_read_cli_config(char* path, size_t size, struct cli_config* buf) {
   struct stat   path_stat;
   FILE*         fin;
   /* libyaml requirements */
   yaml_parser_t parser;
   yaml_event_t  event;
   /* parse depth / path tracking */
   int           depth[40] = { 0 };
   int           depth_index = 0;
   int           depth_of_interest[] = { YAML_STREAM_START_EVENT,
                                         YAML_DOCUMENT_START_EVENT,
                                         YAML_MAPPING_START_EVENT, 0 };
   /* key / value capture */
   char          key[1024] = { 0 };
   /* to compare the current stack (depth) with the (depth_of_interest) */
   int           compare_index;
   int           differ;

   if (buf == 0) {
       return -1;
   }
   memset(buf->ea_php_config, 0, CLI_CONFIG_EA_PHP_CONFIG_SIZE);
   memset(buf->testing_root_dir, 0, CLI_CONFIG_TESTING_ROOT_DIR_SIZE);
   memset(buf->php_bin_pattern, 0, CLI_CONFIG_PHP_BIN_PATTERN_SIZE);
   if (path == 0 || path[0] == 0) {
      return 0;
   }
   if (stat(path, &path_stat) != 0 || S_ISREG(path_stat.st_mode) == 0) {
     return -1;
   } 
   fin = fopen(path, "r");
   if (fin) {
     yaml_parser_initialize(&parser);
     yaml_parser_set_input_file(&parser, fin);

     while (yaml_parser_parse(&parser, &event) &&
            event.type != YAML_STREAM_END_EVENT) {
       switch (event.type) {
         case YAML_STREAM_START_EVENT: case YAML_DOCUMENT_START_EVENT:
         case YAML_MAPPING_START_EVENT: case YAML_SEQUENCE_START_EVENT:
           /* push position to stack, clear key and value buffers */
           depth[depth_index++] = event.type;
           key[0] = 0;
           break;
         case YAML_STREAM_END_EVENT: case YAML_DOCUMENT_END_EVENT:
         case YAML_MAPPING_END_EVENT: case YAML_SEQUENCE_END_EVENT:
           /* clear position from stack, clear key and value buffers */
           depth[--depth_index] = 0;
           key[0] = 0;
           break;
         case YAML_ALIAS_EVENT:
           /* In case someone tries to define a key to an alias, clear both */
           key[0] = 0;
           break;
         case YAML_SCALAR_EVENT:
           /* compare parse position.  Future improvement, put in funciton */
           compare_index = -1;
           differ = 0;
           do {
             compare_index++;
             differ = depth[compare_index] - depth_of_interest[compare_index];
           } while (differ == 0 && depth_of_interest[compare_index] != 0);
           /* If exact depth match and blank key, set the key */
           if (differ == 0 && key[0] == 0) {
             strncpy(key, (const char*)event.data.scalar.value, 1024);
             break;
           }
           /* If exact depth match and blank value, set the value */
           if (strncmp(key, "ea_php_yaml", 40) == 0) {
             strncpy(buf->ea_php_config, (const char*)event.data.scalar.value, CLI_CONFIG_EA_PHP_CONFIG_SIZE);
           } else if (strncmp(key, "testing_root_dir", 40) == 0) {
             strncpy(buf->testing_root_dir, (const char*)event.data.scalar.value, CLI_CONFIG_TESTING_ROOT_DIR_SIZE);
           } else if (strncmp(key, "php_bin_pattern", 40) == 0) {
             strncpy(buf->php_bin_pattern, (const char*)event.data.scalar.value, CLI_CONFIG_PHP_BIN_PATTERN_SIZE);
           } else if (strncmp(key, "lsphp_bin_pattern", 40) == 0) {
             strncpy(buf->lsphp_bin_pattern, (const char*)event.data.scalar.value, CLI_CONFIG_LSPHP_BIN_PATTERN_SIZE);
           };
           break;
         case YAML_NO_EVENT:
           /* to suppress compiler warning on unhandled enumeration value */
           break;
       }
       yaml_event_delete(&event);
     }
     yaml_parser_delete(&parser);
     fclose(fin);
   }
   return 0;
}

void cli_config_load(struct cli_config* cli_config) {
  char conf_file[1024] = { 0 };

  path_php_cli_yaml(conf_file, 1024);
  cli_config_read_cli_config(conf_file, 1024, cli_config);
}

void cli_config_get_php_bin(char* php_bin, size_t size, struct cli_config* cli_config) {
}
