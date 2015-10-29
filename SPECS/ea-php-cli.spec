# Defining the package namespace
%global ns_name ea
%global upstream_name php-cli

%define debug_package %{nil}

Name:           %{ns_name}-%{upstream_name}
Version:        0.0.5
Release:        1%{dist}
Vendor:         cPanel, Inc.
Summary:        Execute PHP scripts with the configured php version.
Url:            http://cpanel.net
License:        GPL
Group:          Development/Languages
BuildRequires:  libyaml-devel
BuildRequires:  gcc-c++
BuildRequires:  libtool
Requires:       libyaml-devel
Requires:       libyaml
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
Source0:        php-cli-0.0.4.tar.gz

%description
php-cli is a program installed to /usr/bin/php that when executed will run
the version of users configured version of php.


%package lsphp
Summary: Execute the correct lsphp binary for the configured PHP version
Group: Development/Languages

%description lsphp
php-cli-lsphp is a program installed to /usr/bin/lsphp that when executed will run
the version of users configured version of lsphp.


%prep
%setup -q -n php-cli-%{version}
mkdir -p %{buildroot}/usr/bin
pwd

%build
#cat Makefile
make php-cli
make php-litespeed


%install
install -m 755 -d $RPM_BUILD_ROOT/usr/bin
install php-cli $RPM_BUILD_ROOT/usr/bin/php
install -m 755 -d $RPM_BUILD_ROOT/usr/local/bin
install php-cli $RPM_BUILD_ROOT/usr/local/bin/php
install php-litespeed $RPM_BUILD_ROOT/usr/local/bin/lsphp
install php-litespeed $RPM_BUILD_ROOT/usr/bin/lsphp

%clean
rm -rf %{buildroot}

%files
%attr(0755,root,root) /usr/bin/php
%attr(0755,root,root) /usr/local/bin/php

%files lsphp
%attr(0755,root,root) /usr/local/bin/lsphp
%attr(0755,root,root) /usr/bin/lsphp

%changelog
* Tue Oct 29 2015 Julian BRown <julian.brown@cpanel.net> - 0.0.5-1
- Cleaned up some minor issues with lsphp
- Cleaned up test code compile issues

* Tue Oct 20 2015 Matt Dees <matt@cpanel.net> - 0.0.4-1
- Add in lsphp support patch created by cloudlinux

* Thu Jul 30 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.3-1
- Minor but needed change in AddType search.

* Tue Jul 28 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.2-2
- Look for AddType instead of AddHandler

* Tue Jul 24 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-2
- Add copy to /usr/local/bin/php

* Tue Jul 21 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-1
- RPM Created

