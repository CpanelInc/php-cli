#!/bin/bash

source debian/vars.sh

set -x

mkdir -p $DEB_INSTALL_ROOT/usr/bin
mkdir -p $DEB_INSTALL_ROOT/usr/local/bin
mkdir -p $DEB_INSTALL_ROOT/var/cpanel/ea4

cp -f $SOURCE0 $DEB_INSTALL_ROOT/usr/bin/php
cp -f $SOURCE1 $DEB_INSTALL_ROOT/usr/local/bin/php
cp -f $SOURCE2 $DEB_INSTALL_ROOT/usr/bin/lsphp
cp -f $SOURCE3 $DEB_INSTALL_ROOT/var/cpanel/ea4/ea_php_cli.pm

