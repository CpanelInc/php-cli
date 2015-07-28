CC=gcc
CFLAGS=-I /usr/local/include
LIBS=-lyaml

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

php-cli: php-cli.o php_yaml.o
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

parse_yaml: parse_yaml.o php_yaml.o
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

parse_conf: parse_conf.o php_yaml.o
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

test_yaml_token: test_yaml_token.o
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

test_yaml: test_yaml.o
	$(CC) $(CFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

all: php-cli test_yaml test_yaml_token parse_yaml parse_conf

