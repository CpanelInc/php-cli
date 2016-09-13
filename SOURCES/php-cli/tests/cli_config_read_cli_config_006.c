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
                          YAML_ALIAS_EVENT,
                          YAML_DOCUMENT_END_EVENT,
                          YAML_STREAM_END_EVENT};
    yaml_char_t* event_values[] = { 0, 0, 0, 0, 0 };
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

  int expected_yaml_parser_parse_called_count = 5;

  printf("testing cli_config_read_cli_config on regular file, containing one document containing an alias\n");
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

  if (strnlen(cli_config.ea_php_config, 8) != 0) {
    printf("ERROR: cli_config.ea_php_config \"%s\" is not empty\n", cli_config.ea_php_config);
    return 1;
  } else {
    printf("  cli_config.ea_php_config \"%s\" is empty\n", cli_config.ea_php_config);
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
