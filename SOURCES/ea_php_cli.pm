#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - ea_php_cli.pm                           Copyright 2019 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

package ea_php_cli;

use strict;
use warnings;

my $PATH_MAX       = 4096;
my $SYSCALL_GETCWD = 79;     # 64bit only

our $EUID;
$EUID = $> if ${^GLOBAL_PHASE} eq "START";

my %types = (
    php       => 'CLI',
    'php-cgi' => 'CGI',
    lsphp     => 'LSAPI',
);

sub run {
    my ( $type, $pkg, $dir, @args ) = proc_args(@_);

    $pkg ||= get_pkg_for_dir( $type, $dir );
    die "Could not determine package for “$dir”\n" if !$pkg;

    return exec_via_pkg( $type, $pkg, @args );
}

sub proc_args {
    my ( $type, @raw_args ) = @_;

    $type //= "undefined";
    die "Invalid type ($type)\n" if !exists $types{$type};

    my ( $pkg, $dir, @args );

    my $idx     = -1;
    my $skipidx = -1;
    my $arg;    # buffer
    for $arg (@raw_args) {
        $idx++;
        next if $idx == $skipidx;

        if ( substr( $arg, 0, 19 ) eq "--ea-reference-dir=" ) {    # --ea-reference-dir=DIR
            ( undef, $dir ) = split( /=/, $arg, 2 );               # if set from -f $arg: blow it away
        }
        elsif ( _file_exists($arg) ) {                             # ZC-11178: use a wrapper in place of -f for testing purposes
            push @args, $arg;

            if ( !defined $dir ) {                                 # set if not already set from --ea-reference-dir
                $dir = $arg;
                if ( index( $dir, "/" ) == -1 ) {
                    $dir = ".";                                    # foo.php
                }
                else {
                    $dir =~ s{/[^/]+$}{};
                    $dir = "/" if $dir eq "";                      # /foo.php
                }
            }
        }
        else {
            push @args, $arg;
        }
    }

    $dir //= ".";

    # since the lookup is based on abs path:
    if ( $dir eq "." ) {
        $dir = _getcwd();
    }
    elsif ( substr( $dir, 0, 1 ) ne "/" || index( $dir, ".." ) != -1 ) {
        require Cwd;
        $dir = Cwd::abs_path($dir);
    }

    die "Could not determine path to lookup PHP setting for based on arguments\n" if !$dir;    # this would be pretty odd

    return ( $type, $pkg, $dir, @args );
}

sub _file_exists { return -f $_[0] }

sub get_pkg_for_dir {
    my ( $type, $dir ) = @_;

    $type //= "undefined";
    die "Invalid type ($type)\n" if !exists $types{$type};

    my $dir_stat = [ stat($dir) ];
    die "“$dir” is not a directory\n" if !-d _;

    return _get_default_pkg() if $dir_stat->[4] == 0;

    my $pkg;
    if ( !$ENV{NO_EA_PHP_CLI_CACHE} ) {
        $pkg = _get_from_cache( $dir, $dir_stat );
        return $pkg if $pkg;
    }
    else {
        _unlink_cache_file( $dir, $dir_stat );
    }

    $pkg = _get_pkg_for_path($dir);

    _cache_it_if_you_can( $dir, $dir_stat, $pkg ) unless $ENV{NO_EA_PHP_CLI_CACHE};

    return $pkg || _get_default_pkg();    # get_php_config_for_users() factors in default, so $pkg should always be set but just in case …

}

sub exec_via_pkg {
    my ( $type, $pkg, @args ) = @_;

    $type //= "undefined";
    die "Invalid type ($type)\n" if !exists $types{$type};

    _pkg_name_check($pkg);

    my $prefix = _get_scl_prefix($pkg);
    my $binary = "$prefix/root/usr/bin/$type";

    if ( !-x $binary ) {
        warn "There is no “$types{$type}” binary for “$pkg”, using default …\n";

        $pkg    = _get_default_pkg();
        $prefix = _get_scl_prefix($pkg);
        $binary = "$prefix/root/usr/bin/$type";

        die "Could not determine binary for “$pkg”\n" if !-x $binary;
    }

    exec( $binary, @args );
    die "Could not execute “$binary”: $!\n";

    return;
}

#################################
#### get_pkg_for_dir() helpers ##
#################################

sub _get_pkg_for_path {
    my ($dir) = @_;
    my $pkg;

    if ( $ENV{'PWD'} && $dir eq $ENV{'PWD'} ) {
        if ( substr( $dir, 0, 1 ) eq '/' ) {
            $pkg = _lookup_pkg_for_path($dir);    # false if the directory they they think they are in is not configured so we can fall back to abspath
        }
        else {
            # this should not be possible naturally i.e. what does PWD of 'i/am/here' mean, what is it relative to? why would it be set to ../bar/../../foo/bar?
            warn "Relative \$PWD detected! Since that can be ambiguous we are ignoring \$PWD value and using absolute path for lookup instead\n";

            # Patches welcome for this rabbit hole ;p file under YAGNI for now
        }

        # no package yet? check the directory they are actually in
        # and call abs_path to resolve any symlinks
        if ( !$pkg ) {
            require Cwd;
            $pkg = _lookup_pkg_for_path( Cwd::abs_path( _getcwd() ) );
        }
    }
    else {
        $pkg = _lookup_pkg_for_path($dir);    # false if the directory they they think they want is not configured so we can fall back to abspath

        if ( !$pkg ) {
            require Cwd;
            my $abs = Cwd::abs_path($dir);

            if ( defined $abs && $abs ne $dir ) {
                $pkg = _lookup_pkg_for_path($abs);
            }
        }
    }

    $pkg = _get_default_pkg() if !$pkg;

    return $pkg;
}

sub _get_from_cache {
    my ( $dir, $dir_stat ) = @_;

    my ( $user, $home ) = ( getpwuid( $dir_stat->[4] ) )[ 0, 7 ];
    my $cachedir = _dir_to_cache_dir( $home, $dir );

    my $pkg;
    if ( my $dir_cache_mtime = ( lstat("$cachedir/.ea-php-cli.cache") )[9] ) {
        my $userdata_cache_mtime = ( stat("/var/cpanel/userdata/$user/cache") )[9];

        if ( !$Cpanel::PHPFPM::Constants::php_conf_path ) { require Cpanel::PHPFPM::Constants }
        my $phpconf_mtime = ( stat($Cpanel::PHPFPM::Constants::php_conf_path) )[9];

        if ( $userdata_cache_mtime && $userdata_cache_mtime < $dir_cache_mtime && $phpconf_mtime < $dir_cache_mtime ) {
            $pkg = readlink("$cachedir/.ea-php-cli.cache");

            eval { _pkg_name_check($pkg); _get_scl_prefix($pkg) };
            warn "$@\n" if $@;

            return $pkg if $pkg;
        }
    }

    return;
}

sub _unlink_cache_file {
    my ( $dir, $dir_stat ) = @_;

    my $home     = ( getpwuid( $dir_stat->[4] ) )[7];
    my $cachedir = _dir_to_cache_dir( $home, $dir );
    unlink("$cachedir/.ea-php-cli.cache");

    return;
}

sub _lookup_pkg_for_path {
    my ($dir) = @_;

    my $pkg;
    my %dir_cache;
    my %uid_cache;
    my ( $dom, $uid );    # buffers
    my %getpwuid_cache;
    require Cpanel::PHP::Config;

    $dir =~ s{/+$}{};     # remove trailing /’s to since lookup is based on not having a trailing slash
    $dir =~ tr{/}{}s;     # squash repetitive sequences of slashes, i.e. //// -> /
    while ($dir) {        # walk the path looking for the first configured PHP (if any)
        $uid = _get_uid($dir);    # EUID may not own $dir
        last if !$uid;            # root can't own a domain, thus can't set a PHP version

        if ( !exists $dir_cache{$dir} ) {
            $getpwuid_cache{$uid} //= [ getpwuid($uid) ];
            eval { $uid_cache{$uid} //= Cpanel::PHP::Config::get_php_config_for_users( [ $getpwuid_cache{$uid}->[0] ] ) };
            last if $@;           # non-cpanel users can't own a domain, thus can't set a PHP version

            for $dom ( keys %{ $uid_cache{$uid} } ) {
                $dir_cache{ $uid_cache{$uid}->{$dom}{documentroot} } = $uid_cache{$uid}->{$dom}{phpversion};
            }
        }

        if ( $dir_cache{$dir} ) {
            $pkg = $dir_cache{$dir};
            last;
        }
        elsif ( defined $getpwuid_cache{$uid} && defined $getpwuid_cache{$uid}->[7] && $dir eq $getpwuid_cache{$uid}->[7] ) {    # because we can cache this one still
            last;
        }

        $dir =~ s{/[^/]+$}{};
    }

    return $pkg;
}

sub _cache_it_if_you_can {
    my ( $dir, $dir_stat, $pkg ) = @_;

    # attempt to cache for non-root
    if ( $EUID && $EUID == $dir_stat->[4] && $pkg ) {    # get_php_config_for_users() factors in default, so $pkg should always be set but just in case …

        local $!;
        unlink "$dir/.ea-php-cli.cache";                 # clean up old cache location if necessary
        my ( $user, $home ) = ( getpwuid( $dir_stat->[4] ) )[ 0, 7 ];
        my $cachedir = _dir_to_cache_dir( $home, $dir );
        require File::Path::Tiny;
        File::Path::Tiny::mk($cachedir) or warn "Could not ensure ea-php-cli cache directory!\n";
        unlink "$cachedir/.ea-php-cli.cache";
        symlink( $pkg, "$cachedir/.ea-php-cli.cache" );
    }
}

sub _dir_to_cache_dir {
    my ( $home, $dir ) = @_;
    $dir =~ s{^\Q$home\E}{$home/.cpanel/ea-php-cli};
    return $dir;
}

###############
#### helpers ##
###############

sub _pkg_name_check {
    my ($pkg) = @_;

    if ( index( $pkg, "/" ) != -1 || index( $pkg, "\0" ) != -1 || index( $pkg, ".." ) != -1 ) {
        $pkg =~ s/\0/{NULL-BYTE}/g;
        die "Package, $pkg, has invalid characters\n";
    }

    return 1;
}

sub _get_default_pkg {
    require Cpanel::PHP::Config;
    my $default_pkg = Cpanel::PHP::Config::get_php_version_info()->{default};    # this calls a cached lookup woot woot
    die "Default PHP is not configured!\n" if !$default_pkg;
    return $default_pkg;
}

my %_get_scl_prefix;

sub _get_scl_prefix {
    my ($pkg) = @_;
    return $_get_scl_prefix{$pkg} if $_get_scl_prefix{$pkg};

    require Cpanel::SysPkgs::SCL;
    my $prefix = Cpanel::SysPkgs::SCL::get_scl_prefix($pkg);
    die "“$pkg” is not an EA4 SCL PHP\n" if !$prefix || !-d $prefix;

    $_get_scl_prefix{$pkg} = $prefix;
    return $_get_scl_prefix{$pkg};
}

sub _get_uid {
    my ($dir) = @_;
    return $EUID || ( stat($dir) )[4];
}

sub _getcwd {
    length( pack( 'l!', 1000 ) ) * 8 == 64 or die "This system only support 64-bit Linux";

    my $cwd            = "\0" x 4096;
    my $syscall_result = syscall( $SYSCALL_GETCWD, $cwd, $PATH_MAX );
    if ( $syscall_result && $syscall_result != -1 ) {
        $cwd =~ tr{\0}{}d;
    }
    else {
        require Cwd;
        $cwd = Cwd::getcwd();
    }

    if ( $ENV{'PWD'} && $cwd ne $ENV{'PWD'} && index( $ENV{'PWD'}, '../' ) == -1 ) {
        require Cwd;
        my $abs_path = Cwd::abs_path( $ENV{'PWD'} );
        if ( $abs_path eq $cwd && ( stat($abs_path) )[4] == ( stat($cwd) )[4] ) {

            # If the absolute path of $ENV{'PWD'} points the the directory we are
            # currently in and it has the same owner than we except $ENV{'PWD'}
            # as truthy
            return $ENV{'PWD'};
        }
    }

    return $cwd;
}

1;

__END__

=head1 This is not intended to be consumed outside of the package that installed it. The functions are meant to consolidate logic and facilitate testing. For usage see L<https://go.cpanel.net/ea-php-cli>.
