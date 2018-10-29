#!/usr/local/cpanel/3rdparty/bin/perl
# cpanel - ea_php_cli.pm                           Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

package ea_php_cli;

use strict;
use warnings;

use Cwd                  ();
use Cpanel::PHP::Config  ();
use Cpanel::SysPkgs::SCL ();

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

        if ( $arg eq '-ea_php' ) {    # -ea_php NN

            # EA-7961 will remove support for this flag
            warn "-ea_php is deprecated (will be removed 2019-11), please use the version specific symlink instead (documented at https://go.cpanel.net/ea-php-cli)\n";
            $pkg     = "ea-php" . $raw_args[ $idx + 1 ];
            $skipidx = $idx + 1;
        }
        elsif ( substr( $arg, 0, 19 ) eq "--ea-reference-dir=" ) {    # --ea-reference-dir=DIR
            ( undef, $dir ) = split( /=/, $arg, 2 );                  # if set from -f $arg: blow it away
        }
        elsif ( -f $arg ) {
            push @args, $arg;

            if ( !defined $dir ) {                                    # set if not already set from --ea-reference-dir
                $dir = $arg;
                if ( index( $dir, "/" ) == -1 ) {
                    $dir = ".";                                       # foo.php
                }
                else {
                    $dir =~ s{/[^/]+$}{};
                    $dir = "/" if $dir eq "";                         # /foo.php
                }
            }
        }
        else {
            push @args, $arg;
        }
    }

    $dir //= ".";
    $dir = Cwd::abs_path($dir);                                       # since the lookup is based on abs path

    die "Could not determine path to lookup PHP setting for based on arguments\n" if !$dir;    # this would be pretty odd

    return ( $type, $pkg, $dir, @args );
}

sub get_pkg_for_dir {
    my ( $type, $dir ) = @_;

    $type //= "undefined";
    die "Invalid type ($type)\n" if !exists $types{$type};

    die "“$dir” is not a directory\n" if !-d $dir;

    my $pkg;

    # ¿TODO/YAGNI? - caching: $pkg = readlink("$dir/.ea-php-cli.cache") if its not too old

    # TODO: optimize me:
    my %dir_cache;
    my %uid_cache;
    my ( $dom, $uid );    # buffers
    my %getpwuid_cache;

    while ($dir) {        # walk the path looking for the first configured PHP (if any)
        $uid = _get_uid($dir);    # EUID may not own $dir
        last if !$uid;            # root can't own a domain, thus can't set a PHP version

        if ( !exists $dir_cache{$dir} ) {
            $getpwuid_cache{$uid} //= [ getpwuid($uid) ];
            $uid_cache{$uid} //= Cpanel::PHP::Config::get_php_config_for_users( [ $getpwuid_cache{$uid}->[0] ] );
            for $dom ( keys %{ $uid_cache{$uid} } ) {
                $dir_cache{ $uid_cache{$uid}->{$dom}{documentroot} } = $uid_cache{$uid}->{$dom}{phpversion};
            }
        }

        if ( $dir_cache{$dir} ) {
            $pkg = $dir_cache{$dir};
            last;
        }

        $dir =~ s{/[^/]+$}{};
    }

    # ¿TODO/YAGNI? - caching: symlink($pkg, "$dir/.ea-php-cli.cache")
    return $pkg || _get_default_pkg();
}

sub exec_via_pkg {
    my ( $type, $pkg, @args ) = @_;

    $type //= "undefined";
    die "Invalid type ($type)\n" if !exists $types{$type};

    if ( index( $pkg, "/" ) != -1 || index( $pkg, "\0" ) != -1 ) {
        $pkg =~ s/\0/{NULL-BYTE}/g;
        die "Package, $pkg, has invalid characters\n";
    }

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

###############
#### helpers ##
###############

sub _get_default_pkg {
    my $default_pkg = Cpanel::PHP::Config::get_php_version_info()->{default};    # this calls a cached lookup woot woot
    die "Default PHP is not configured!\n" if !$default_pkg;
    return $default_pkg;
}

sub _get_scl_prefix {
    my ($pkg) = @_;

    my $prefix = Cpanel::SysPkgs::SCL::get_scl_prefix($pkg);
    die "“$pkg” is not an EA4 SCL PHP\n" if !$prefix || !-d $prefix;

    return $prefix;
}

sub _get_uid {
    my ($dir) = @_;
    return $> || ( stat($dir) )[4];
}

1;

__END__

=head1 This is not intended to be consumed outside of the package that installed it. The functions are meant to consolidate logic and facilitate testing. For usage see L<https://go.cpanel.net/ea-php-cli>.
