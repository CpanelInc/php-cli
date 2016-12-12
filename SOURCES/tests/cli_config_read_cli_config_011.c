/* ea-php-cli - tests/cli_config_read_cli_config_011.c  Copyright 2016 cPanel, Inc. */
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

#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>

#include "cli-config.h"

int xstat_called = 0;

int __wrap___xstat(int ver, const char* path, struct stat* buf) {
   xstat_called = 1;
   buf->st_mode = S_IFREG;
   return 0;
}

int fopen_called = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
   fopen_called = 1;
   return (FILE*)5;
}

int fclose_called = 0;

int __wrap_fclose(FILE* file) {
   fclose_called = 1;
   return 0;
}

int yaml_parser_init_called = 0;

int __wrap_yaml_parser_initialize(yaml_parser_t* parser) {
    yaml_parser_init_called = 1;
    return 1;
}

int yaml_parser_set_input_file_called = 0;

void __wrap_yaml_parser_set_input_file(yaml_parser_t* parser, FILE* file) {
    yaml_parser_set_input_file_called = 1;
}

int yaml_parser_parse_call_count = 0;

int __wrap_yaml_parser_parse(yaml_parser_t* parser, yaml_event_t* event) {
    int event_types[] = { YAML_STREAM_START_EVENT,
                          YAML_DOCUMENT_START_EVENT,
                          YAML_MAPPING_START_EVENT,
                          YAML_SCALAR_EVENT,
                          YAML_SCALAR_EVENT,
			  YAML_MAPPING_END_EVENT,
                          YAML_DOCUMENT_END_EVENT,
                          YAML_STREAM_END_EVENT};
    yaml_char_t* event_values[] = { 0, 0, 0, (yaml_char_t*)"ea_php_yaml",
                                   (yaml_char_t*)"my_value", 0, 0, 0 };
    if (yaml_parser_parse_call_count > 
        (int)(sizeof(event_types)/sizeof(event_types[0]))) {
      event->type = YAML_NO_EVENT;
      event->data.scalar.value = 0;
      yaml_parser_parse_call_count++;
      return 0;
    } else {
      event->type = event_types[yaml_parser_parse_call_count];
      event->data.scalar.value = event_values[yaml_parser_parse_call_count];
      yaml_parser_parse_call_count++;
      return 1;
    }
}

int yaml_event_delete_called = 0;

void __wrap_yaml_event_delete(yaml_event_t* event) {
    yaml_event_delete_called = 1;
}

int yaml_parser_delete_called = 0;

void __wrap_yaml_parser_delete(yaml_parser_t* parser) {
    yaml_parser_delete_called = 1;
}

int main(int argc, char** argv) {
  struct cli_config cli_config;
  char   file[1024] = "/a/regular/file.txt";
  int    return_code = 0;

  /* Assure garbage in cli_config */
  strncpy(cli_config.ea_php_config, "unset", 10);
  strncpy(cli_config.testing_root_dir, "unset", 10);
  strncpy(cli_config.php_bin_pattern, "unset", 10);

  int expected_yaml_parser_parse_called_count = 8;

  printf("testing cli_config_read_cli_config on regular file, containing one document containing an mapping for ea_php_yaml\n");
  printf("  calling cli_config_read_cli_config(\"%s\", %d, &cli_config\n", file, 1024);

  return_code = cli_config_read_cli_config(file, 1024, &cli_config);

  if (return_code != 0) {
    printf("ERROR: return code %d is not zero\n", return_code);
    return 1;
  } else {
    printf("  return code %d is zero\n", return_code);
  }

  if (xstat_called == 0) {
    printf("ERROR: no attempt to detect type of file\n");
    return 1;
  } else {
    printf("  attempt to detect type of file\n");
  }

  if (fopen_called == 0) {
    printf("ERROR: no attempt to open file\n");
    return 1;
  } else {
    printf("  fopen called\n");
  }

  if (yaml_parser_init_called == 0) {
    printf("ERROR: no attempt to initialized yaml parser\n");
    return 1;
  } else {
    printf("  yaml_parser_initialize called\n");
  }

  if (yaml_parser_set_input_file_called == 0) {
    printf("ERROR: no attempt to set input file for yaml parser\n");
    return 1;
  } else {
    printf("  yaml_parser_set_input_file called\n");
  }

  if (yaml_parser_parse_call_count != expected_yaml_parser_parse_called_count) {
    printf("ERROR: yaml_parser_parse called %d times, expected %d\n",
           yaml_parser_parse_call_count, expected_yaml_parser_parse_called_count);
    return 1;
  } else {
    printf("  yaml_parser_parse all count is correct\n");
  }

  if (yaml_event_delete_called == 0) {
    printf("ERROR: no attempt to cleanup / delete yaml events\n");
    return 1;
  } else {
    printf("  yaml_event_delete called\n");
  }

  if (yaml_parser_delete_called == 0) {
    printf("ERROR: no attempt to cleanup / delete yaml parser\n");
    return 1;
  } else {
    printf("  yaml_parser_delete called\n");
  }

  if (fclose_called == 0) {
    printf("ERROR: no attempt to close file\n");
    return 1;
  } else {
    printf("  fclose called\n");
  }

  if (strncmp(cli_config.ea_php_config, "my_value", 1024) != 0) {
    printf("ERROR: cli_config.ea_php_config \"%s\" is not set to \"my_value\"\n", cli_config.ea_php_config);
    return 1;
  } else {
    printf("  cli_config.ea_php_config \"%s\" is set correctly to \"my_value\"\n", cli_config.ea_php_config);
  }

  if (strnlen(cli_config.testing_root_dir, 8) != 0) {
    printf("ERROR: cli_config.testing_root_dir \"%s\" is not empty\n", cli_config.testing_root_dir);
    return 1;
  } else {
    printf("  cli_config.testing_root_dir \"%s\" is empty\n", cli_config.testing_root_dir);
  }

  if (strnlen(cli_config.php_bin_pattern, 8) != 0) {
    printf("ERROR: cli_config.php_bin_pattern \"%s\" is not empty\n", cli_config.php_bin_pattern);
    return 1;
  } else {
    printf("  cli_config.php_bin_pattern \"%s\" is empty\n", cli_config.php_bin_pattern);
  }

  printf("test complete\n");
  return 0;
}
