#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - t/install-PHPCLI.t                      Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

## no critic qw(TestingAndDebugging::RequireUseStrict TestingAndDebugging::RequireUseWarnings)
use Test::Spec;    # automatically turns on strict and warnings

use FindBin;

our @system_calls;
our $current_system = sub { push @system_calls, [@_]; return 0; };
use Test::Mock::Cmd 'system' => sub { $current_system->(@_) };

use Test::MockFile qw/strict/;

use Cpanel::FileUtils::Copy ();
our @copy_args;

use Capture::Tiny ();

BEGIN { require "$FindBin::Bin/../SOURCES/PHPCLI.pm" }

our %mocks;

describe "CLI PHP JIT compiler for ULC/install/" => sub {
    before "all" => sub {
        for my $bin ( keys %Install::PHPCLI::files ) {
            my $src = $Install::PHPCLI::files{$bin};
            $mocks{$bin} = Test::MockFile->file( $bin, $bin );
            $mocks{$src} = Test::MockFile->file( $src, $src ) if !exists $mocks{$src};
        }
    };

    describe "new()" => sub {
        it "should return an Install::PHPCLI object" => sub {
            isa_ok( Install::PHPCLI->new, 'Install::PHPCLI' );
        };

        it "should return a Cpanel::Task based object" => sub {
            isa_ok( Install::PHPCLI->new, 'Cpanel::Task' );
        };
    };

    describe "perform()" => sub {
        around {
            no warnings "redefine";
            local @copy_args;
            local *Cpanel::FileUtils::Copy::safecopy = sub { push @copy_args, \@_; 1 };
            local *Capture::Tiny::capture_merged = sub { $_[0]->() };
            yield;
        };

        it "should output what it is doing for each binary" => sub {
            trap { Install::PHPCLI->new->perform() };
            is $trap->stdout, join( "", map { "JIT Compiling $_\n" } sort keys %Install::PHPCLI::files );
        };

        it "should die if a source is missing" => sub {

            # getting a missing src file takes a little work since this does not work:
            # local $mocks{'/var/cpanel/ea4/ea-php-cli/php-cgi.pl'} = undef;
            my $src = '/var/cpanel/ea4/ea-php-cli/php-cgi.pl';
            $mocks{$src}->unlink();

            trap { Install::PHPCLI->new->perform() };
            $mocks{$src}->contents($src);

            is $trap->die, "/usr/bin/php source file went missing, reinstall the ea-php-cli RPM\n";
        };

        it "should perlcc each binary" => sub {
            local @system_calls;
            trap { Install::PHPCLI->new->perform() };
            my $perlcc = "/usr/local/cpanel/3rdparty/bin/perlcc";
            is_deeply \@system_calls,
              [
                map { [ $perlcc, "-o", $_, $Install::PHPCLI::files{$_} ] }
                sort keys %Install::PHPCLI::files
              ];
        };

        it "should, on perlcc unclean exit, warn about it" => sub {
            local $current_system = sub { 1 };
            trap { Install::PHPCLI->new->perform() };
            like $trap->stderr, qr/JIT compile .* failed,/;
        };

        it "should, on perlcc unclean exit, install the source version" => sub {
            local $current_system = sub { 1 };
            trap { Install::PHPCLI->new->perform() };
            is_deeply \@copy_args,
              [
                map { [ $Install::PHPCLI::files{$_}, $_ ] }
                sort keys %Install::PHPCLI::files
              ];
        };

        it "should warn when source version install fails" => sub {
            local $current_system = sub { 1 };
            no warnings "redefine";
            local *Cpanel::FileUtils::Copy::safecopy = sub { 0 };
            trap { Install::PHPCLI->new->perform() };
            like $trap->stderr, qr/Installation of uncompiled .* failed/;
        };
    };
};

runtests unless caller;
