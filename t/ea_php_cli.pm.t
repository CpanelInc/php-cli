#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - t/ea_php_cli.pm.t                       Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

## no critic qw(TestingAndDebugging::RequireUseStrict TestingAndDebugging::RequireUseWarnings)
use Test::Spec;    # automatically turns on strict and warnings

our @exec_calls;
our $current_exec = sub { push @exec_calls, [@_]; return 0; };
use Test::Mock::Cmd 'exec' => sub { $current_exec->(@_) };

use FindBin;
use lib "$FindBin::Bin/../SOURCES";
use_ok "ea_php_cli";

use Cwd ();
use Path::Tiny 'path';
use Cpanel::PHP::Config ();

our %test = (
    cur_func => undef,
    sec_arg  => "",      # this arg should cause $cur_func to fail after type check w/out doing anything
);

share %test;

describe "CLI PHP module" => sub {
    spec_helper ".spec_helpers/all_type_funcs.pl";

    describe "proc_args() function" => sub {
        before all => sub { $test{cur_func} = 'ea_php_cli::proc_args' };
        it_should_behave_like "all functions that take a type as first arg";

        it "should return the valid type that was passed in from wrapper as first value" => sub {
            is [ ea_php_cli::proc_args("php-cgi") ]->[0], "php-cgi";
        };

        it "should return undef pkg as second value when no pkg flag was given" => sub {
            is [ ea_php_cli::proc_args("php-cgi") ]->[1], undef;
        };

        it "should return pkg when given via -ea_php NN" => sub {
            my @rv;
            trap { @rv = ea_php_cli::proc_args( "php-cgi", -ea_php => 99 ) };
            is $rv[1], "ea-php99";
        };

        it "should give deprecation warning when given -ea_php NN" => sub {
            trap { ea_php_cli::proc_args( "php-cgi", -ea_php => 99 ) };
            is $trap->stderr, "-ea_php is deprecated (will be removed 2019-11), please use the version specific symlink instead (documented at https://go.cpanel.net/ea-php-cli)\n";
        };

        it "should return, as its third value, abs path of . when given no file" => sub {
            is [ ea_php_cli::proc_args("php-cgi") ]->[2], Cwd::abs_path(".");
        };

        it "should return, as its third value, abs path of relative ./ file" => sub {
            is [ ea_php_cli::proc_args( "php-cgi", "./php.php" ) ]->[2], Cwd::abs_path(".");
        };

        it "should return, as its third value, abs path of relative ../../ file" => sub {
            is [ ea_php_cli::proc_args( "php-cgi", "../../php.php" ) ]->[2], Cwd::abs_path("../..");
        };

        it "should return, as its third value, abs path of relative file w/out path parts" => sub {
            is [ ea_php_cli::proc_args( "php-cgi", "php.php" ) ]->[2], Cwd::abs_path(".");
        };

        it "should return, as its third value, abs path of --ea-reference-dir value when it exists" => sub {
            is [ ea_php_cli::proc_args( "php-cgi", "--ea-reference-dir=/usr/local/cpanel", "php.php" ) ]->[2], "/usr/local/cpanel";
        };

        it "should return, as its third value, abs path of --ea-reference-dir value when it does not exist" => sub {
            is [ ea_php_cli::proc_args( "php-cgi", "--ea-reference-dir=./derp$$", "php.php" ) ]->[2], Cwd::abs_path("./derp$$");
        };

        it "should pass everything but -ea_php NN, and --ea-reference-dir after the 3rd value" => sub {
            my @args = qw(1 2 3 -derp file.php -xyz);
            my @rv;
            trap { @rv = ea_php_cli::proc_args( "php-cgi", qw(-ea_php NN), @args, "--ea-reference-dir=DIR" ) };
            is_deeply \@rv, [ "php-cgi", "ea-phpNN", Cwd::abs_path("DIR"), @args ];
        };
    };

    describe "get_pkg_for_dir() function" => sub {
        before all => sub { $test{cur_func} = 'ea_php_cli::get_pkg_for_dir' };
        it_should_behave_like "all functions that take a type as first arg";

        around {
            no warnings "redefine";
            local $test{get_php_config_for_users}       = {};
            local $test{get_php_config_for_users_count} = 0;
            local $test{_get_uid}                       = $$;
            local $test{tmpdir}                         = Path::Tiny->tempdir();
            local $test{get_php_version_info}           = { default => "i-am-default-hear-me-roar-$$" };
            local *Cpanel::PHP::Config::get_php_config_for_users = sub { $test{get_php_config_for_users_count}++; $test{get_php_config_for_users} };
            local *Cpanel::PHP::Config::get_php_version_info     = sub { $test{get_php_version_info} };
            local *ea_php_cli::_get_uid                          = sub { $test{_get_uid} };
            yield;
        };

        it "should die when given a non-dir" => sub {
            trap { ea_php_cli::get_pkg_for_dir( "php-cgi", "i-am-not-a-dir-$$" ) };
            is $trap->die, "“i-am-not-a-dir-$$” is not a directory\n";
        };

        it "should die when there is nothing set for the path and no default" => sub {
            local $test{get_php_version_info} = {};
            trap { ea_php_cli::get_pkg_for_dir( "php-cgi", $test{tmpdir} ) };
            is $trap->die, "Default PHP is not configured!\n";
        };

        it "should go immediately to default pkg when run as root and dir is not owned by a user" => sub {
            local $test{_get_uid} = 0;
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $test{tmpdir} );
            is_deeply [ $pkg, $test{get_php_config_for_users_count} ], [ "i-am-default-hear-me-roar-$$", 0 ];
        };

        it "should get pkg configured for given dir" => sub {
            path("$test{tmpdir}/$$/foo/bar")->mkpath;
            local $test{get_php_config_for_users} = {
                ffdna => {
                    documentroot => "$test{tmpdir}/$$/foo/bar",
                    phpversion   => "ea-php-bar",
                },
                fqdnb => {
                    documentroot => "$test{tmpdir}/$$/foo",
                    phpversion   => "ea-php-foo",
                },
            };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}/$$/foo/bar" );
            is $pkg, "ea-php-bar";
        };

        it "should get pkg configured for given dir's parent" => sub {
            path("$test{tmpdir}/$$/foo/bar")->mkpath;
            local $test{get_php_config_for_users} = {
                fqdnb => {
                    documentroot => "$test{tmpdir}/$$/foo",
                    phpversion   => "ea-php-foo",
                },
            };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}/$$/foo/bar" );
            is $pkg, "ea-php-foo";
        };

        it "should go to default if nothing is configured anywhere on the path" => sub {
            path("$test{tmpdir}/$$/foo/bar")->mkpath;
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}/$$/foo/bar" );
            is_deeply [ $pkg, $test{get_php_config_for_users_count} ], [ "i-am-default-hear-me-roar-$$", 1 ];
        };
    };

    describe "exec_via_pkg() function" => sub {
        before all => sub { $test{cur_func} = 'ea_php_cli::exec_via_pkg' };
        it_should_behave_like "all functions that take a type as first arg";

        around {
            local $test{tmpdir} = Path::Tiny->tempdir();
            path("$test{tmpdir}/PKG/root/usr/bin")->mkpath;
            path("$test{tmpdir}/DEF/root/usr/bin")->mkpath;

            no warnings "redefine";
            local *ea_php_cli::_get_default_pkg = sub { "DEF" };
            local *ea_php_cli::_get_scl_prefix  = sub { "$test{tmpdir}/$_[0]" };

            yield;
        };

        it "should die if given a package name w/ a / in it" => sub {
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "ea-php/lolhaxorz", 1, 2, 3 ) };
            is $trap->die, "Package, ea-php/lolhaxorz, has invalid characters\n";
        };

        it "should die if given a package w/ a NULL byte in it" => sub {
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "ea-php\0lolhaxorz", 1, 2, 3 ) };
            is $trap->die, "Package, ea-php{NULL-BYTE}lolhaxorz, has invalid characters\n";
        };

        it "should warn about trying default if the binary for the given package is missing" => sub {
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "ea-php$$", 1, 2, 3 ) };
            is $trap->stderr, "There is no “CGI” binary for “ea-php$$”, using default …\n";
        };

        it "should die if it goes to default if the binary for the default package is missing" => sub {
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "ea-php$$", 1, 2, 3 ) };
            is $trap->die, "Could not determine binary for “DEF”\n";
        };

        it "should exec the given package when it has the binary" => sub {
            path("$test{tmpdir}/DEF/root/usr/bin/php-cgi")->spew("# oh hai $$");
            chmod 0755, "$test{tmpdir}/DEF/root/usr/bin/php-cgi";
            path("$test{tmpdir}/PKG/root/usr/bin/php-cgi")->spew("# oh hai $$");
            chmod 0755, "$test{tmpdir}/PKG/root/usr/bin/php-cgi";

            local @exec_calls = ();
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "PKG", 1, 2, 3 ) };
            is_deeply \@exec_calls, [ [ "$test{tmpdir}/PKG/root/usr/bin/php-cgi", 1, 2, 3 ] ];
        };

        it "should exec the default package when the given package doesn't have the binary and the default does" => sub {
            path("$test{tmpdir}/DEF/root/usr/bin/php-cgi")->spew("# oh hai $$");
            chmod 0755, "$test{tmpdir}/DEF/root/usr/bin/php-cgi";

            local @exec_calls = ();
            trap { ea_php_cli::exec_via_pkg( "php-cgi", "PKG", 1, 2, 3 ) };
            is_deeply \@exec_calls, [ [ "$test{tmpdir}/DEF/root/usr/bin/php-cgi", 1, 2, 3 ] ];
        };
    };

    describe "run() function" => sub {
        before all => sub { $test{cur_func} = 'ea_php_cli::run' };
        it_should_behave_like "all functions that take a type as first arg";

        it "should pass all args to proc_args()" => sub {
            my @proc_args;
            no warnings "redefine";
            local *ea_php_cli::proc_args = sub {
                @proc_args = @_;
                return ( "php-cgi", "ea-php$$", ".", "args", "to", "php" );
            };
            trap { ea_php_cli::run( "php-cgi", "args", "to", "php" ) };
            is_deeply \@proc_args, [ "php-cgi", "args", "to", "php" ];
        };

        it "should call get_pkg_for_dir() when args did not indicate a package" => sub {
            no warnings "redefine";
            my @pkg_args;
            local *ea_php_cli::proc_args = sub { return ( "php-cgi", undef, ".", "args", "to", "php" ) };
            local *ea_php_cli::get_pkg_for_dir = sub { @pkg_args = @_; return "ea-php$$" };
            trap { ea_php_cli::run( "php-cgi", "args", "to", "php" ) };
            is_deeply \@pkg_args, [ "php-cgi", "." ];    # not Cwd::abs_path(".") since we're mocking
        };

        it "should not call get_pkg_for_dir() when args did indicate a package" => sub {
            no warnings "redefine";
            my @pkg_args;
            local *ea_php_cli::proc_args = sub { return ( "php-cgi", "ea-php$$", ".", "args", "to", "php" ) };
            local *ea_php_cli::get_pkg_for_dir = sub { @pkg_args = @_; return "ea-php$$" };
            trap { ea_php_cli::run( "php-cgi", "args", "to", "php" ) };
            is_deeply \@pkg_args, [];
        };

        it "die when get_pkg_for_dir() is called but it can’t find a package" => sub {
            no warnings "redefine";
            my @pkg_args;
            local *ea_php_cli::proc_args = sub { return ( "php-cgi", undef, ".", "args", "to", "php" ) };
            local *ea_php_cli::get_pkg_for_dir = sub { @pkg_args = @_; return; };
            trap { ea_php_cli::run( "php-cgi", "args", "to", "php" ) };
            is $trap->die, "Could not determine package for “.”\n";    # not Cwd::abs_path(".") since we're mocking

        };

        it "should call exec_via_pkg() as expected" => sub {
            no warnings "redefine";
            my @exec_args;
            local *ea_php_cli::proc_args = sub { return ( "php-cgi", "ea-php$$", ".", "args", "to", "php" ) };
            local *ea_php_cli::exec_via_pkg = sub { @exec_args = @_ };
            trap { ea_php_cli::run( "php-cgi", "args", "to", "php" ) };
            is_deeply \@exec_args, [ "php-cgi", "ea-php$$", "args", "to", "php" ];
        };
    };
};

runtests unless caller;
