# AC_PREREQ(2.59)
AC_INIT([libnatspec], [0.0.1], lav@etersoft.ru)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME,AC_PACKAGE_VERSION)

AC_CONFIG_HEADER(config.h)

dnl Initialize maintainer mode
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB


dnl Initialize libtool
AC_DISABLE_STATIC
AC_ENABLE_SHARED(yes)
AC_ENABLE_STATIC(no)
AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([popt], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([nl_langinfo setlocale strcasecmp strchr])

AC_CONFIG_FILES([Makefile
                 lib/Makefile
                 src/Makefile
				 tests/Makefile])

AC_OUTPUT
