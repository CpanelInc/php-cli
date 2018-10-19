dnl ea-php-cli - Makefile.am                  Copyright 2016 cPanel, Inc.
dnl                                                  All rights Reserved.
dnl copyright@cpanel.net                                http://cpanel.net
dnl
dnl Licensed under the Apache License, Version 2.0 (the "License");
dnl you may not use this file except in compliance with the License.
dnl You may obtain a copy of the License at
dnl 
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl 
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.

AC_INIT([php-cli], [0.2.1], [support@cpanel.net])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([am-tools])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADER([config.h])
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_CHECK_LIB([yaml], [yaml_get_version], [], [AC_MSG_ERROR([libyaml not found])])

AC_CONFIG_MACRO_DIR([m4])
AM_PROG_AR
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_RANLIB
AC_COVERAGE

AH_TOP([
/* ea-php-cli - config.h / config.h.in          Copyright 2016 cPanel, Inc. */
/*                                                     All rights Reserved. */
/* copyright@cpanel.net                                   http://cpanel.net */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the "License");          */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an "AS IS" BASIS,        */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
])

echo $COVERAGE_CFLAGS
echo $COVERAGE_OPTFLAGS
echo $LCOV

AC_CONFIG_FILES([
  Makefile
  include/Makefile
  src/Makefile
  tests/Makefile
])
AC_OUTPUT
