AC_INIT([libstatsdc], [0.3], [mina.libstatsdc@naguib.ca])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile libstatsdc/Makefile src/Makefile])
AC_CONFIG_SRCDIR([libstatsdc/statsdc.h])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([socket strdup strerror])

AM_INIT_AUTOMAKE([-Wall -Werror])
AM_PROG_AR
LT_INIT

AC_OUTPUT
