AC_INIT([php-cli], [0.1.0], [support@cpanel.net])
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

AC_CONFIG_FILES([
  Makefile
  include/Makefile
  src/Makefile
  tests/Makefile
])
AC_OUTPUT
