package Install::PHPCLI;

# cpanel - install/EAPHPCLI.pm                     Copyright 2018 cPanel, L.L.C.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

use base qw( Cpanel::Task );

use strict;
use warnings;
use Capture::Tiny ();

our $VERSION = '1.0';

=head1 DESCRIPTION

JIT compilation of ea-php-cli scripts.

ATM this is required because OBS does not have any cPanel code.

ZC-4417 is about addressing that if possible

=over 1

=item Type: Install and Update in order to tie in the current ULC code

=item Frequency: install and update

=item EOL: never unless ZC-4417 is doable

=back

=cut

sub new {
    my $proto = shift;
    my $self  = $proto->SUPER::new;

    $self->set_internal_name('phpcli');

    return $self;
}

our %files = (
    '/usr/bin/php'         => '/var/cpanel/ea4/ea-php-cli/php-cgi.pl',
    '/usr/local/bin/php'   => '/var/cpanel/ea4/ea-php-cli/php-cli.pl',
    '/usr/bin/lsphp'       => '/var/cpanel/ea4/ea-php-cli/php-lsapi.pl',
    '/usr/local/bin/lsphp' => '/var/cpanel/ea4/ea-php-cli/php-lsapi.pl',
);

sub perform {
    my $self = shift;

    for my $bin ( sort keys %files ) {
        next if $bin =~ m{/lsphp} && !-e $files{$bin};    # lsphp ones are not always installed

        my $sysrv;
        print "JIT Compiling $bin\n";

        # perlcc exits clean when given a bad input file so just in case the user has manually goofed up the RPM files:
        die "$bin source file went missing, reinstall the ea-php-cli RPM\n" if !-e $files{$bin};

        my $o = Capture::Tiny::capture_merged(
            sub {                                         # hide scary sounding yet harmless warning from users on C7, its a know issue CM-1223
                $sysrv = system( "/usr/local/cpanel/3rdparty/bin/perlcc", '-o', $bin, $files{$bin} );
            }
        );

        if ( $sysrv != 0 ) {
            warn "JIT compile $bin failed, output was:\n\t$o\nUsing uncompiled $bin …\n";
            Cpanel::FileUtils::Copy::safecopy( $files{$bin}, $bin ) || warn "Installation of uncompiled $bin failed: $!\n";
        }

        if ( !-x $bin ) {
            chmod( 0755, $bin ) or warn "Could not make $bin executable: $!\n";
        }
    }

    return 1;
}

Install::PHPCLI->new->perform() if !caller();

1;
