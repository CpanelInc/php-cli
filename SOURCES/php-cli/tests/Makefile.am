TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) tap-driver.sh

AM_CFLAGS = -I$(top_srcdir)/include
LDADD = ../src/libphpcli.a

TEST_CASES = \
cli_get_php_version_001 \
cli_get_php_version_002 \
cli_get_php_version_003 \
cli_get_php_version_004 \
cli_get_php_version_005 \
cli_get_last_file_001 \
cli_get_last_file_002 \
cli_get_last_file_003 \
cli_get_last_file_004 \
cli_get_last_file_005 \
cli_get_last_file_006 \
cli_get_last_file_007 \
cli_config_read_cli_config_001 \
cli_config_read_cli_config_002 \
cli_config_read_cli_config_003 \
cli_config_read_cli_config_004 \
cli_config_read_cli_config_005 \
cli_config_read_cli_config_006 \
cli_config_read_cli_config_007 \
cli_config_read_cli_config_008 \
cli_config_read_cli_config_009 \
cli_config_read_cli_config_010 \
cli_config_read_cli_config_011 \
cli_config_read_cli_config_012 \
cli_config_read_cli_config_013 \
cli_config_read_cli_config_014 \
htaccess_get_php_version_from_file_001 \
htaccess_get_php_version_from_file_002 \
htaccess_get_php_version_from_file_003 \
htaccess_get_php_version_from_file_004 \
htaccess_get_php_version_from_file_005 \
htaccess_get_php_version_from_file_006 \
htaccess_get_php_version_from_file_007 \
htaccess_get_php_version_from_file_008 \
path_basedir_001 \
path_basedir_002 \
path_basedir_003 \
path_basedir_004 \
path_basedir_005 \
path_basedir_006 \
path_php_cli_yaml_001 \
path_php_cli_yaml_002 \
path_php_cli_yaml_003 \
path_php_cli_yaml_004 \
path_php_cli_yaml_005 \
path_get_htaccess_php_version_001 \
path_get_htaccess_php_version_002 \
path_get_htaccess_php_version_003 \
path_get_htaccess_php_version_004 \
path_get_htaccess_php_version_005 \
path_get_htaccess_php_version_006 \
path_get_htaccess_php_version_007 \
path_get_htaccess_php_version_008 \
path_get_htaccess_php_version_009 \
strategy_get_lsphp_bin_001 \
strategy_get_lsphp_bin_002 \
strategy_get_lsphp_bin_003 \
strategy_get_lsphp_bin_004 \
strategy_get_php_bin_001 \
strategy_get_php_bin_002 \
strategy_get_php_bin_003 \
strategy_get_php_bin_004 \
strategy_get_php_bin_005 \
strategy_get_php_bin_006 \
strategy_get_php_bin_007 \
strategy_get_php_bin_008 \
strategy_get_php_bin_009 \
strategy_get_php_bin_010 \
strategy_get_php_bin_011 \
strategy_get_php_bin_012 \
strategy_get_php_bin_013 \
strategy_get_php_bin_014 \
strategy_get_php_bin_015 \
strategy_get_php_bin_016 \
strategy_get_php_bin_017 \
strategy_get_php_bin_018 \
strategy_get_php_bin_019

TESTS = $(TEST_CASES)

AM_CLFAGS = -O0

check_PROGRAMS = $(TEST_CASES) parse_conf parse_yaml test_yaml test_yaml_token

cli_get_last_file_003_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_004_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_005_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_006_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_007_LDFLAGS   = -Wl,--wrap=access
htaccess_get_php_version_from_file_001_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_002_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_003_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_004_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_version_from_file_008_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
path_basedir_001_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_002_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_003_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_005_LDFLAGS        = -Wl,--wrap=getcwd
path_php_cli_yaml_001_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_002_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_003_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_004_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_005_LDFLAGS   = -Wl,--wrap=readlink
path_get_htaccess_php_version_001_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_002_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_003_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_004_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_005_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_006_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_007_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_008_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
path_get_htaccess_php_version_009_LDFLAGS = -Wl,--wrap=htaccess_get_php_version_from_file
cli_config_read_cli_config_001_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
cli_config_read_cli_config_002_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
cli_config_read_cli_config_003_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
cli_config_read_cli_config_004_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_008_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_009_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_010_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_011_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_012_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_013_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
cli_config_read_cli_config_014_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=yaml_parser_initialize,--wrap=yaml_parser_set_input_file,--wrap=yaml_parser_parse,--wrap=yaml_event_delete,--wrap=yaml_parser_delete
strategy_get_lsphp_bin_004_LDFLAGS = -Wl,--wrap=__xstat
strategy_get_php_bin_004_LDFLAGS = -Wl,--wrap=__xstat
strategy_get_php_bin_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_008_LDFLAGS = -Wl,--wrap=__xstat
strategy_get_php_bin_009_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_010_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_011_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_012_LDFLAGS = -Wl,--wrap=__xstat
strategy_get_php_bin_013_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_014_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_015_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_016_LDFLAGS = -Wl,--wrap=__xstat
strategy_get_php_bin_017_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_018_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_bin_019_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access

parse_conf_LDADD = ../src/libphpcli.a

parse_yaml_LDADD = ../src/libphpcli.a

test_yaml_LDADD = ../src/libphpcli.a

test_yaml_token_SOURCES = test_yaml_token.c
test_yaml_token_LDADD = ../src/libphpcli.a
