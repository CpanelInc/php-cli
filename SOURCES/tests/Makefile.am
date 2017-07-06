## ea-php-cli - tests/Makefile.am            Copyright 2017 cPanel, Inc.
##                                                  All rights Reserved.
## copyright@cpanel.net                                http://cpanel.net
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
## 
##     http://www.apache.org/licenses/LICENSE-2.0
## 
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.

TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) tap-driver.sh

AM_CFLAGS = -I$(top_srcdir)/include
LDADD = ../src/libphpcli.a

TEST_CASES = \
cli_get_php_package_001 \
cli_get_php_package_002 \
cli_get_php_package_003 \
cli_get_php_package_004 \
cli_get_php_package_005 \
cli_get_last_file_001 \
cli_get_last_file_002 \
cli_get_last_file_003 \
cli_get_last_file_004 \
cli_get_last_file_005 \
cli_get_last_file_006 \
cli_get_last_file_007 \
cli_get_verbose_001 \
cli_get_verbose_002 \
cli_get_verbose_003 \
cli_get_verbose_004 \
cli_get_verbose_005 \
cli_get_verbose_006 \
cli_get_verbose_007 \
cli_get_verbose_008 \
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
htaccess_get_php_package_from_file_001 \
htaccess_get_php_package_from_file_002 \
htaccess_get_php_package_from_file_003 \
htaccess_get_php_package_from_file_004 \
htaccess_get_php_package_from_file_005 \
htaccess_get_php_package_from_file_006 \
htaccess_get_php_package_from_file_007 \
htaccess_get_php_package_from_file_008 \
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
path_get_htaccess_php_package_001 \
path_get_htaccess_php_package_002 \
path_get_htaccess_php_package_003 \
path_get_htaccess_php_package_004 \
path_get_htaccess_php_package_005 \
path_get_htaccess_php_package_006 \
path_get_htaccess_php_package_007 \
path_get_htaccess_php_package_008 \
path_get_htaccess_php_package_009 \
paths_config_read_paths_config_001 \
paths_config_read_paths_config_002 \
paths_config_read_paths_config_003 \
paths_config_read_paths_config_004 \
paths_config_read_paths_config_005 \
paths_config_read_paths_config_006 \
paths_config_read_paths_config_007 \
paths_config_read_paths_config_008 \
paths_config_read_paths_config_009 \
paths_config_read_paths_config_010 \
paths_config_read_paths_config_011 \
paths_config_read_paths_config_012 \
paths_config_read_paths_config_013 \
paths_config_read_paths_config_014 \
paths_config_read_paths_config_015 \
paths_config_read_paths_config_016 \
paths_config_read_paths_config_017 \
paths_config_read_paths_config_018 \
paths_config_read_paths_config_019 \
paths_config_read_paths_config_020 \
paths_config_read_paths_config_021 \
paths_config_read_paths_config_022 \
paths_config_read_paths_config_023 \
paths_config_read_paths_config_024 \
paths_config_read_paths_config_025 \
paths_config_read_paths_config_026 \
paths_config_read_paths_config_027 \
paths_config_read_paths_config_028 \
paths_config_read_paths_config_029 \
paths_config_read_paths_config_030 \
paths_config_read_paths_config_031 \
paths_config_read_paths_config_032 \
paths_config_read_paths_config_033 \
paths_config_read_paths_config_034 \
paths_config_read_paths_config_035 \
paths_config_read_paths_config_036 \
paths_config_read_paths_config_037 \
paths_config_read_paths_config_038 \
paths_config_read_paths_config_039 \
paths_config_read_paths_config_040 \
paths_config_read_paths_config_041 \
paths_config_read_paths_config_042 \
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
strategy_get_php_bin_019 \
strategy_get_php_conf_file_001 \
strategy_get_php_conf_file_002 \
strategy_get_php_conf_file_003 \
strategy_get_php_conf_file_004 \
strategy_get_php_conf_file_005 \
strategy_get_php_conf_file_006 \
strategy_get_php_conf_file_007 \
strategy_get_php_conf_file_008 

TESTS = $(TEST_CASES)

AM_CLFAGS = -O0

check_PROGRAMS = $(TEST_CASES) parse_yaml test_yaml test_yaml_token

cli_get_last_file_003_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_004_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_005_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_006_LDFLAGS   = -Wl,--wrap=access
cli_get_last_file_007_LDFLAGS   = -Wl,--wrap=access
htaccess_get_php_package_from_file_001_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_002_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_003_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_004_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
htaccess_get_php_package_from_file_008_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fgets,--wrap=fclose
path_basedir_001_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_002_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_003_LDFLAGS        = -Wl,--wrap=getcwd
path_basedir_005_LDFLAGS        = -Wl,--wrap=getcwd
path_php_cli_yaml_001_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_002_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_003_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_004_LDFLAGS   = -Wl,--wrap=readlink
path_php_cli_yaml_005_LDFLAGS   = -Wl,--wrap=readlink
path_get_htaccess_php_package_001_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_002_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_003_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_004_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_005_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_006_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_007_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_008_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
path_get_htaccess_php_package_009_LDFLAGS = -Wl,--wrap=htaccess_get_php_package_from_file
paths_config_read_paths_config_001_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
paths_config_read_paths_config_002_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
paths_config_read_paths_config_003_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
paths_config_read_paths_config_004_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose
paths_config_read_paths_config_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_008_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_009_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_010_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_011_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_012_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_013_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_014_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_015_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_016_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_017_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_018_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_019_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_020_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_021_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_022_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_023_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_024_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_025_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_026_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_027_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_028_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_029_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_030_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_031_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_032_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_033_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_034_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_035_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_036_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_037_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_038_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_039_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_040_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_041_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
paths_config_read_paths_config_042_LDFLAGS = -Wl,--wrap=__xstat,--wrap=fopen,--wrap=fclose,--wrap=fgets
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
strategy_get_php_conf_file_004_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_conf_file_005_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_conf_file_006_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_conf_file_007_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access
strategy_get_php_conf_file_008_LDFLAGS = -Wl,--wrap=__xstat,--wrap=access

parse_yaml_LDADD = ../src/libphpcli.a

test_yaml_LDADD = ../src/libphpcli.a

test_yaml_token_SOURCES = test_yaml_token.c
test_yaml_token_LDADD = ../src/libphpcli.a
