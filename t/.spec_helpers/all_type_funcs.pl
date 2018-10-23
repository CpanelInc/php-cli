## cpanel - t/.spec_helpers/all_type_funcs.pl       Copyright 2018 cPanel, L.L.C.
##                                                           All rights Reserved.
## copyright@cpanel.net                                         http://cpanel.net
## This code is subject to the cPanel license. Unauthorized copying is prohibited
#
### no critic qw(TestingAndDebugging::RequireUseStrict TestingAndDebugging::RequireUseWarnings)
use Test::Spec;    # automatically turns on strict and warnings

shared_examples_for "all functions that take a type as first arg" => sub {
    share my %test;

    it "should die about an unknown type if not given one" => sub {
        trap { no strict "refs"; $test{cur_func}->() };
        is $trap->die, "Invalid type (undefined)\n";
    };

    it "should die about an unknown type if given an undef one" => sub {
        trap { no strict "refs"; $test{cur_func}->(undef) };
        is $trap->die, "Invalid type (undefined)\n";
    };

    it "should die about an unknown type if given an empty one" => sub {
        trap { no strict "refs"; $test{cur_func}->("") };
        is $trap->die, "Invalid type ()\n";
    };

    it "should die about an unknown type if given an unknown one" => sub {
        trap { no strict "refs"; $test{cur_func}->("derp") };
        is $trap->die, "Invalid type (derp)\n";
    };

    it "should not die about an unknown type if given 'php'" => sub {
        trap { no strict "refs"; $test{cur_func}->( "php", $test{sec_arg} ) };
        unlike $trap->die, qr/Invalid type/;
    };

    it "should not die about an unknown type if given 'php-cgi'" => sub {
        trap { no strict "refs"; $test{cur_func}->( "php-cgi", $test{sec_arg} ) };
        unlike $trap->die, qr/Invalid type/;
    };

    it "should not die about an unknown type if given 'lsapi'" => sub {
        trap { no strict "refs"; $test{cur_func}->( "php-cgi", $test{sec_arg} ) };
        unlike $trap->die, qr/Invalid type/;
    };
};

