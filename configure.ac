# Configure ac for LIBNATSPEC project
#
AC_PREREQ(2.52)
AC_INIT([libnatspec], [0.1.1], lav@etersoft.ru)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME,AC_PACKAGE_VERSION)
dnl AS_VERSION(comedilib, COMEDILIB, 0, 7, 21)
dnl AS_LIBTOOL(COMEDILIB, 0, 7, 18, yes)

AC_CONFIG_HEADER(config.h)
CFLAGS="-Wall -funsigned-char"

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
#AC_PROG_LIBTOOL


dnl Initialize libtool
AC_DISABLE_STATIC
AC_ENABLE_SHARED(yes)
AC_ENABLE_STATIC(no)
AM_PROG_LIBTOOL
dnl AC_PROG_LIBTOOL

dnl Checks for libraries.
check_popt=""
AC_CHECK_LIB([popt], [main], check_popt=true)
if test "x$check_popt" == x ; then
	AC_MSG_ERROR([*** libpopt not found.])
else
	POPT_LIBS=-lpopt
	AC_SUBST(POPT_LIBS)
	AC_DEFINE(HAVE_LIBPOPT,1,[Define if you have the POPT library])
fi


dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([nl_langinfo setlocale strcasecmp strchr])

dnl Python
AC_ARG_WITH(python, [  --with-python    Compile python binding ],
[AM_PATH_PYTHON(2.3)
AM_CHECK_PYTHON_HEADERS(HAVE_PYTHON=yes,[HAVE_PYTHON=no;AC_MSG_WARN([python headers not found, disabling python binding])])
])
AM_CONDITIONAL(HAVE_PYTHON, [test "x$HAVE_PYTHON" = xyes])

dnl #AS_COMPILER_FLAG([-fno-strict-aliasing],[PYTHON_QUIET="$PYTHON_QUIET -fno-strict-aliasing"], true )
dnl #AS_COMPILER_FLAG([-Wno-unused-function],[PYTHON_QUIET="$PYTHON_QUIET -Wno-unused-function"], true )
dnl #AC_SUBST(PYTHON_QUIET)


AC_ARG_WITH(debug, [  --with-debug    Compile with debug information], CFLAGS="$CFLAGS -g")


AC_CONFIG_FILES([Makefile
				 libnatspec.pc
                 lib/Makefile
                 libhi/Makefile
                 src/Makefile
				 src/python/Makefile
				 tests/Makefile])

AC_OUTPUT
