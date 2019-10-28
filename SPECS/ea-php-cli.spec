# Defining the package namespace
%global ns_name ea
%global upstream_name php-cli
%define debug_package %{nil}

Name:           %{ns_name}-%{upstream_name}
Version:        1.0.0
# Doing release_prefix this way for Release allows for OBS-proof versioning, See EA-4566 for more details
%define release_prefix 9
Release: %{release_prefix}%{?dist}.cpanel
Vendor:         cPanel, Inc.
Summary:        Execute PHP scripts with the configured php version.
Url:            http://cpanel.net
License:        ASL 2.0
Group:          Development/Languages
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
AutoReq:        no
Source0:        php-cgi.pl
Source1:        php-cli.pl
Source2:        php-lsapi.pl
Source3:        ea_php_cli.pm

%description
CLI and CGI PHP wrappers to dispatch to the user's configured version of php.

%package lsphp
Summary: Execute the correct lsphp binary for the configured PHP version
Group: Development/Languages
Requires: ea-php-cli = %{version}-%{release}

%description lsphp
LSAPI PHP wrappers to dispatch to the user's configured version of php.

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/bin
mkdir -p %{buildroot}/usr/local/bin
mkdir -p %{buildroot}/var/cpanel/ea4
cp -f %SOURCE0 %{buildroot}/usr/bin/php
cp -f %SOURCE1 %{buildroot}/usr/local/bin/php
cp -f %SOURCE2 %{buildroot}/usr/bin/lsphp
cp -f %SOURCE2 %{buildroot}/usr/local/bin/lsphp
cp -f %SOURCE3 %{buildroot}/var/cpanel/ea4/ea_php_cli.pm

%clean
rm -rf %{buildroot}

%files
%attr(0755,root,root) /usr/bin/php
%attr(0755,root,root) /usr/local/bin/php
%attr(0644,root,root) /var/cpanel/ea4/ea_php_cli.pm

%files lsphp
%attr(0755,root,root) /usr/local/bin/lsphp
%attr(0755,root,root) /usr/bin/lsphp

%changelog
* Tue Oct 22 2019 Tim Mullin <tim@cpanel.net> - 1.0.0-9
- EA-7961: Remove deprecated -ea_php flag

* Tue Jul 23 2019 Tim Mullin <tim@cpanel.net> - 1.0.0-8
- EA-8537: Fix php version detection when cwd is a symlink

* Fri Mar 01 2019 Tim Mullin <tim@cpanel.net> - 1.0.0-7
- EA-8257: Fix warning when PWD environment variable not set

* Mon Feb 18 2019 Daniel Muey <dan@cpanel.net> - 1.0.0-6
- ZC-4806: Smarter handling of paths containing symlinks

* Fri Feb 15 2019 Tim Mullin <tim@cpanel.net> - 1.0.0-5
- EA-8227: Fix _lookup_pkg_for_path to handle multiple consecutive slashes

* Mon Jan 21 2019 Daniel Muey <dan@cpanel.net> - 1.0.0-4
- ZC-4712: Move cache into ~/.cpanel/

* Wed Dec 19 2018 Daniel Muey <dan@cpanel.net> - 1.0.0-3
- ZC-4580: handle two minor edge cases

* Mon Dec 10 2018 Daniel Muey <dan@cpanel.net> - 1.0.0-2
- ZC-4459: Cache the calculated PHP version for a given directory

* Tue Oct 23 2018 Daniel Muey <dan@cpanel.net> - 1.0.0-1
- ZC-4400: rewrite to make it easier to work with and fix all the bugs
- ZC-4425: deprecate -ea_php flag

* Thu Oct 18 2018 Rikus Goodell <rikus.goodell@cpanel.net> - 0.2.1-1
- EA-7935: Add support for -ea_reference_dir option.

* Wed Aug 09 2017 Cory McIntire <cory@cpanel.net> - 0.2.0-7
- EA-6669: Adjust line_copy buffer to prevent overflow

* Tue Jul 25 2017 Dan Muey <dan@cpanel.net> - 0.2.0-6
- redo tarball *AGAIN*, we really need to do EA-6572

* Thu Jul 20 2017 Dan Muey <dan@cpanel.net> - 0.2.0-5
- EA-6569: fix `-ea_php NN` handling to set package name
- EA-6542: Ignore commented out MIME Types
- EA-6543: ignore .htaccess 'mime type packages' that does not contain '-php'

* Tue Jun 20 2017 Dan Muey <dan@cpanel.net> - 0.2.0-4
- EA-6333: Support MultiPHP System's non ea- SCL PHPs

* Wed Dec 14 2016 Edwin Buck <e.buck@cpanel.net> - 0.2.0-1
- EA-5741: Improve php-cli handling of paths.conf

* Fri Sep 16 2016 Edwin Buck <e.buck@cpanel.net> - 0.1.0-2
- EA-5212: Update license and copyright information

* Tue Sep 06 2016 Edwin Buck <e.buck@cpanel.net> - 0.1.0-1
- EA-5110: Add /usr/local/bin/php targets to php-cli
           Change /usr/bin/php targets to php-cgi

* Wed Aug 24 2016 Jacob Perkins <jacob.perkins@cpanel.net> - 0.0.6-5
- EA-5120: Revert php-cli wrapper back to php-cli until EA-5106 is complete

* Mon Aug 01 2016 Dan Muey <dan@cpanel.net> - 0.0.6-4
- EA-4905: Update EA4 php-cli wrapper to use php-cgi as a temporary fix for EA-4337

* Mon Jun 20 2016 Dan Muey <dan@cpanel.net> - 0.0.6-3
- EA-4383: Update Release value to OBS-proof versioning

* Fri Dec 11 2015 S. Kurt Newman <kurt.newman@cpanel.net> - 0.0.6-1
- Supports new MultiPHP php.conf location

* Thu Oct 29 2015 Julian BRown <julian.brown@cpanel.net> - 0.0.5-1
- Cleaned up some minor issues with lsphp
- Cleaned up test code compile issues

* Tue Oct 20 2015 Matt Dees <matt@cpanel.net> - 0.0.4-1
- Add in lsphp support patch created by cloudlinux

* Thu Jul 30 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.3-1
- Minor but needed change in AddType search.

* Tue Jul 28 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.2-2
- Look for AddType instead of AddHandler

* Fri Jul 24 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-2
- Add copy to /usr/local/bin/php

* Tue Jul 21 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-1
- RPM Created

