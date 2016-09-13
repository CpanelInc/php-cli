my_execbindir = /usr/bin
my_execlocalbindir = /usr/local/bin

AM_CFLAGS=-I$(top_srcdir)/include

LDADD = ./libphpcli.a

noinst_LIBRARIES = libphpcli.a
libphpcli_a_SOURCES = cli.c cli-config.c htaccess.c path.c php_yaml.c \
                      strategy.c

my_execbin_PROGRAMS = php-cgi php-lightspeed
my_execlocalbin_PROGRAMS = php-cli

php_cgi_SOURCES = php-cgi.c
php_cli_SOURCES = php-cli.c
php_lightspeed_SOURCES = php-lightspeed.c

