#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - php-cgi                                 Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

package ea_php_cli::cgi;

use strict;
use warnings;

use lib '/var/cpanel/ea4';
use ea_php_cli;

my $bin = "php-cgi";
run(@ARGV) unless caller();

sub run {
    return ea_php_cli::run( $bin, @_ );    # faster than unshift @_, "php-cgi"; goto &ea_php_cli::run; ?
}

1;
