#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - t/ea_php_cli.pm.t                       Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

## no critic qw(TestingAndDebugging::RequireUseStrict TestingAndDebugging::RequireUseWarnings)
use Test::Spec;    # automatically turns on strict and warnings
use vars '$HOME';

BEGIN {
    no warnings "redefine", "once";
    *CORE::GLOBAL::getpwuid = sub { wantarray ? ( "user$$", "x", $$ + 1, $$ + 1, "", "", "", $HOME ) : ("user$$") }
}

our @exec_calls;
our $current_exec = sub { push @exec_calls, [@_]; return 0; };
use Test::Mock::Cmd 'exec' => sub { $current_exec->(@_) };

use FindBin;
use lib "$FindBin::Bin/../SOURCES";
use_ok "ea_php_cli";

use Cwd ();
use Path::Tiny 'path';
use Cpanel::PHP::Config ();
use Test::MockFile;

our %test = (
    cur_func => undef,
    sec_arg  => "",      # this arg should cause $cur_func to fail after type check w/out doing anything
);

share %test;
my $tmp = Path::Tiny->tempdir();
$HOME = $tmp;
$test{tmpdir} = Test::MockFile->dir( "$tmp/foo/ea_php_cli.pm.d", [], { uid => $$ + 1 } );

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
            my $mock = Test::MockFile->file( Cwd::abs_path("../..") . "/php.php", "oh hai" );
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
            $test{tmpdir}->contents( [] );
            local $test{get_php_version_info} = { default => "i-am-default-hear-me-roar-$$" };
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
            trap { ea_php_cli::get_pkg_for_dir( "php-cgi", $test{tmpdir}->{file_name} ) };
            is $trap->die, "Default PHP is not configured!\n";
        };

        it "should go immediately to default pkg when run as root and dir is not owned by a user" => sub {
            local $test{_get_uid} = 0;
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $test{tmpdir}->{file_name} );
            is_deeply [ $pkg, $test{get_php_config_for_users_count} ], [ "i-am-default-hear-me-roar-$$", 0 ];
        };

        it "should get pkg configured for given dir" => sub {
            my $foo = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo",     [], { uid => $$ + 1 } );
            my $bar = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo/bar", [], { uid => $$ + 1 } );

            local $test{get_php_config_for_users} = {
                ffdna => {
                    documentroot => "$test{tmpdir}->{file_name}/$$/foo/bar",
                    phpversion   => "ea-php-bar",
                },
                fqdnb => {
                    documentroot => "$test{tmpdir}->{file_name}/$$/foo",
                    phpversion   => "ea-php-foo",
                },
            };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}->{file_name}/$$/foo/bar" );
            is $pkg, "ea-php-bar";
        };

        it "should get pkg configured for given dir's parent" => sub {
            my $foo = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo",     [], { uid => $$ + 1 } );
            my $bar = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo/bar", [], { uid => $$ + 1 } );
            local $test{get_php_config_for_users} = {
                fqdnb => {
                    documentroot => "$test{tmpdir}->{file_name}/$$/foo",
                    phpversion   => "ea-php-foo",
                },
            };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}->{file_name}/$$/foo/bar" );
            is $pkg, "ea-php-foo";
        };

        it "should go to default if nothing is configured anywhere on the path" => sub {
            my $foo = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo",     [], { uid => $$ + 1 } );
            my $bar = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$/foo/bar", [], { uid => $$ + 1 } );
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", "$test{tmpdir}->{file_name}/$$/foo/bar" );
            is_deeply [ $pkg, $test{get_php_config_for_users_count} ], [ "i-am-default-hear-me-roar-$$", 1 ];
        };
    };

    describe "get_pkg_for_dir() caching" => sub {
        around {
            no warnings "once";
            local $ea_php_cli::EUID = $$;
            yield;
        };

        it "should cache in HOME/cpanel/ea-php-cli/ when it is in EUID's HOME" => sub {
            my $dir = Test::MockFile->dir( "/home/derp/foo/bar/baz", [] );
            is ea_php_cli::_dir_to_cache_dir( "/home/derp", "/home/derp/foo/bar/baz" ), "/home/derp/.cpanel/ea-php-cli/foo/bar/baz";
        };

        it "should not cache in HOME/cpanel/ea-php-cli/ when it is not in EUID's HOME" => sub {
            my $dir = Test::MockFile->dir( "/usr/share/foo/bar/baz", [] );
            is ea_php_cli::_dir_to_cache_dir( "/home/derp", "/usr/share/foo/bar/baz" ), "/usr/share/foo/bar/baz";
        };

        it "should return default package if DIR is owned by root" => sub {
            my $dir = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$", [], { uid => 0 } );
            no warnings "redefine";
            local *ea_php_cli::_dir_to_cache_dir = sub { $dir->{file_name} };
            local *ea_php_cli::_get_default_pkg  = sub { "DEF$$" };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $dir->{file_name} );
            is $pkg, "DEF$$";
        };

        it "should return default if DIR is owned by non-cpanel users" => sub {
            my $dir = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$", [], { uid => $$ + 42 } );
            no warnings "redefine";
            local *ea_php_cli::_dir_to_cache_dir                 = sub { $dir->{file_name} };
            local *Cpanel::PHP::Config::get_php_config_for_users = sub { die "Failed to fetch userdata for “derp” at …" };
            local *ea_php_cli::_get_default_pkg                  = sub { "DEF$$" };
            my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $dir->{file_name} );
            is $pkg, "DEF$$";
        };

        it "should return DIR cache if it is newer than the DIR owner's user data" => sub {
            my $dir = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$", [], { uid => $$ + 1 } );
            no warnings "redefine";
            local *ea_php_cli::_dir_to_cache_dir = sub { $dir->{file_name} };

            # Test::MockFile->symlink() is broken ATM: https://github.com/CpanelInc/Test-MockFile/issues/25
            # my $cch = Test::MockFile->symlink( "ea-php88", "$dir->{file_name}/.ea-php-cli.cache" );
            path( $dir->{file_name} )->mkpath;
            symlink( "ea-php88", "$dir->{file_name}/.ea-php-cli.cache" );
            my $cache_mtime = ( lstat("$dir->{file_name}/.ea-php-cli.cache") )[9];
            my $udr = Test::MockFile->file( "/var/cpanel/userdata/user$$/cache", "", { mtime => $cache_mtime - 42 } );
            my $pkg;
            trap { $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $dir->{file_name} ) };
            is $pkg, "ea-php88";
        };

        describe ", when cache is older than the DIR owner's user data," => sub {
            around {
                no warnings "redefine";
                local *Cpanel::PHP::Config::get_php_config_for_users = sub {
                    return {
                        ffdna => {
                            documentroot => "$test{tmpdir}->{file_name}/$$",
                            phpversion   => "ea-php-wop",
                        },
                      },
                      ;
                };

                local $test{cached_dir} = Test::MockFile->dir( "$test{tmpdir}->{file_name}/$$", [], { uid => $$ + 1 } );
                local *ea_php_cli::_dir_to_cache_dir = sub { $test{cached_dir}->{file_name} };

                # Test::MockFile->symlink() is broken ATM: https://github.com/CpanelInc/Test-MockFile/issues/25
                path( $test{cached_dir}->{file_name} )->mkpath;
                unlink "$test{cached_dir}->{file_name}/.ea-php-cli.cache";
                symlink( "ea-php88", "$test{cached_dir}->{file_name}/.ea-php-cli.cache" );
                my $cache_mtime = ( lstat("$test{cached_dir}->{file_name}/.ea-php-cli.cache") )[9];

                local $test{userdata_cache} = Test::MockFile->file( "/var/cpanel/userdata/user$$/cache", "", { mtime => $cache_mtime + 42 } );

                yield;
            };

            it "should lookup and return the DIR setting" => sub {
                my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $test{cached_dir}->{file_name} );
                is $pkg, "ea-php-wop";
            };

            it "should cache the lookup if the EUID owns the DIR" => sub {
                local $ea_php_cli::EUID = $$ + 1;
                my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $test{cached_dir}->{file_name} );
                is readlink("$test{cached_dir}->{file_name}/.ea-php-cli.cache"), "ea-php-wop";
            };

            it "should NOT cache the lookup if the EUID does not own the DIR" => sub {
                my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $test{cached_dir}->{file_name} );
                is readlink("$test{cached_dir}->{file_name}/.ea-php-cli.cache"), "ea-php88";
            };

            it "should cache the default under EUID when DIR is HOME" => sub {
                local $ea_php_cli::EUID = $$ + 1;
                local $HOME             = $test{tmpdir}->{file_name};
                no warnings "redefine";
                local *ea_php_cli::_dir_to_cache_dir = sub { $HOME };
                local *ea_php_cli::_get_default_pkg  = sub { "DEF$$" };
                my $pkg = ea_php_cli::get_pkg_for_dir( "php-cgi", $HOME );
                is readlink("$HOME/.ea-php-cli.cache"), "DEF$$";
            };
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
