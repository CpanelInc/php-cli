# ea-php-cli.spec                                 Copyright(c) 2015 cPanel, Inc.
#                                                           All rights Reserved.
# copyright@cpanel.net                                         http://cpanel.net
# This code is subject to the cPanel license. Unauthorized copying is prohibited

# Defining the package namespace
%global ns_name ea
%global upstream_name php-cli

%define debug_package %{nil}

Name:           %{ns_name}-%{upstream_name}
Version:        0.0.1
Release:        2%{dist}
Vendor:         cPanel, Inc.
Summary:        Execute PHP scripts with the configured php version.
Url:            http://cpanel.net
License:        GPL
Group:          System Environment/Daemons
BuildRequires:  libyaml-devel
BuildRequires:  gcc-c++
BuildRequires:  libtool
Requires:       libyaml-devel
Requires:       libyaml
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
Source0:        php-cli-0.0.1.tar.gz

%description
php-cli is a program installed to /usr/bin/php that when executed will run
the version of users configured version of php.

%prep
%setup -q -n php-cli-%{version}
mkdir -p %{buildroot}/usr/bin
pwd

%build
#cat Makefile
make php-cli

%install
install -m 755 -d $RPM_BUILD_ROOT/usr/bin
install php-cli $RPM_BUILD_ROOT/usr/bin/php
install -m 755 -d $RPM_BUILD_ROOT/usr/local/bin
install php-cli $RPM_BUILD_ROOT/usr/local/bin/php

%clean
rm -rf %{buildroot}

%files
%attr(0755,root,root) /usr/bin/php
%attr(0755,root,root) /usr/local/bin/php

%changelog
* Tue Jul 28 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.2-2
- Look for AddType instead of AddHandler

* Tue Jul 24 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-2
- Add copy to /usr/local/bin/php

* Tue Jul 21 2015 Julian Brown <julian.brown@cpanel.net> - 0.0.1-1
- RPM Created

