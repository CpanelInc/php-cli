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
   buf->st_mode = S_IFDIR;
   return 0;
}

int unauthorized_call_fopen = 0;

FILE* __wrap_fopen(const char* path, const char* mode) {
   unauthorized_call_fopen = 1;
   return 0;
}

int unauthorized_call_fclose = 0;

int __wrap_fclose(FILE* file) {
   unauthorized_call_fclose = 1;
   return 0;
}

int unauthorized_yaml_parser_init_call = 0;

int __wrap_yaml_parser_initialize(yaml_parser_t* parser) {
    unauthorized_yaml_parser_init_call = 1;
    return 1;
}

int unauthorized_yaml_parser_set_input_file_call = 0;

void __wrap_yaml_parser_set_input_file(yaml_parser_t* parser, FILE* file) {
    unauthorized_yaml_parser_set_input_file_call = 1;
}

int yaml_parser_parse_called_count = 0;

int __wrap_yaml_parser_parse(yaml_parser_t* parser, yaml_event_t* event) {
    yaml_parser_parse_called_count++;
    return 0;
}

int unauthorized_yaml_event_delete_call = 0;

void __wrap_yaml_event_delete(yaml_event_t* event) {
    unauthorized_yaml_event_delete_call = 1;
}

int unauthorized_yaml_parser_delete_call = 0;

void __wrap_yaml_parser_delete(yaml_parser_t* parser) {
    unauthorized_yaml_parser_delete_call = 1;
}

int main(int argc, char** argv) {
  struct cli_config cli_config;
  char   file[1024] = "/a/regular/file.txt";
  int    return_code = 0;

  /* Assure garbage in cli_config */
  strncpy(cli_config.ea_php_config, "unset", 10);
  strncpy(cli_config.testing_root_dir, "unset", 10);
  strncpy(cli_config.php_bin_pattern, "unset", 10);

  int expected_yaml_parser_parse_called_count = 0;

  printf("testing cli_config_read_cli_config on directory\n");
  printf("  calling cli_config_read_cli_config(\"%s\", %d, &cli_config\n", file, 1024);

  return_code = cli_config_read_cli_config(file, 1024, &cli_config);

  if (return_code == 0) {
    printf("ERROR: return code %d is zero\n", return_code);
    return 1;
  } else {
    printf("  return code %d is not zero\n", return_code);
  }

  if (xstat_called == 0) {
    printf("ERROR: no attempt to detect type of file\n");
    return 1;
  } else {
    printf("  attempt to detect type of file\n");
  }

  if (unauthorized_call_fopen) {
    printf("ERROR: attempt to open path occurred\n");
    return 1;
  } else {
    printf("  fopen not called\n");
  }

  if (unauthorized_call_fclose) {
    printf("ERROR: attempt to close path occurred\n");
    return 1;
  } else {
    printf("  fclose not called\n");
  }

  if (unauthorized_yaml_parser_init_call) {
    printf("ERROR: yaml_parser_init called\n");
    return 1;
  } else {
    printf("  yaml_parser_init not called\n");
  }

  if (unauthorized_yaml_parser_set_input_file_call) {
    printf("ERROR: yaml_parser_set_input_file called\n");
    return 1;
  } else {
    printf("  yaml_parser_set_input_file not called\n");
  }

  if (yaml_parser_parse_called_count != expected_yaml_parser_parse_called_count) {
    printf("ERROR: yaml_parser_parse_called_count called %d times, expected %d\n",
           yaml_parser_parse_called_count, expected_yaml_parser_parse_called_count);
    return 1;
  } else {
    printf("  yaml_parser_parse_called_count call count is correct\n");
  }

  if (unauthorized_yaml_event_delete_call) {
    printf("ERROR: yaml_event_delete called\n");
    return 1;
  } else {
    printf("  yaml_event_delete not called\n");
  }

  if (unauthorized_yaml_parser_delete_call) {
    printf("ERROR: yaml_parser_delete called\n");
    return 1;
  } else {
    printf("  yaml_parser_delete not called\n");
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
