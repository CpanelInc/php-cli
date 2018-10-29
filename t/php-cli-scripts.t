#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - t/php-cli-scripts.t                     Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

## no critic qw(TestingAndDebugging::RequireUseStrict TestingAndDebugging::RequireUseWarnings)
use Test::Spec;    # automatically turns on strict and warnings

use FindBin;
use lib "$FindBin::Bin/../SOURCES/";
use ea_php_cli;

{
    local $SIG{__WARN__} = sub { };
    require "$FindBin::Bin/../SOURCES/php-cgi.pl";
    require "$FindBin::Bin/../SOURCES/php-cli.pl";
    require "$FindBin::Bin/../SOURCES/php-lsapi.pl";
}

describe "CLI PHP script" => sub {
    describe "php-cgi" => sub {
        it "should dispatch w/ `php-cgi` as first arg" => sub {
            is _run( "cgi", 1, 2, 3 )->[0], "php-cgi";
        };

        it "should pass other args through" => sub {
            is_deeply _run( "cgi", 4, 5, 6 ), [ "php-cgi", 4, 5, 6 ];
        };
    };
    describe "php-cli" => sub {
        it "should dispatch w/ `php` as first arg" => sub {
            is _run( "cli", 1, 2, 3 )->[0], "php";
        };

        it "should pass other args through" => sub {
            is_deeply _run( "cli", 4, 5, 6 ), [ "php", 4, 5, 6 ];
        };
    };
    describe "php-lsapi" => sub {
        it "should dispatch w/ `lsphp` as first arg" => sub {
            is _run( "lsapi", 1, 2, 3 )->[0], "lsphp";
        };

        it "should pass other args through" => sub {
            is_deeply _run( "lsapi", 4, 5, 6 ), [ "lsphp", 4, 5, 6 ];
        };
    };
};

runtests unless caller;

###############
#### helpers ##
###############

sub _run {
    my ( $ns, @given_args ) = @_;

    my @passed_args;
    no warnings "redefine";
    local *ea_php_cli::run = sub { @passed_args = @_ };
    no strict "refs";
    "ea_php_cli::${ns}::run"->(@given_args);
    return \@passed_args;
}
