#!/bin/bash

source debian/vars.sh

set -x

mkdir -p $DEB_INSTALL_ROOT/usr/bin
mkdir -p $DEB_INSTALL_ROOT/usr/local/bin
mkdir -p $DEB_INSTALL_ROOT/var/cpanel/ea4

install -m 755 $SOURCE0 $DEB_INSTALL_ROOT/usr/bin/php
install -m 755 $SOURCE1 $DEB_INSTALL_ROOT/usr/local/bin/php
install -m 755 $SOURCE2 $DEB_INSTALL_ROOT/usr/bin/lsphp
install -m 644 $SOURCE3 $DEB_INSTALL_ROOT/var/cpanel/ea4/ea_php_cli.pm

