# AC_PREREQ(2.59)
AC_INIT([libnatspec], [0.0.3], lav@etersoft.ru)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME,AC_PACKAGE_VERSION)
#AS_VERSION(comedilib, COMEDILIB, 0, 7, 21)
#AS_LIBTOOL(COMEDILIB, 0, 7, 18, yes)

AC_CONFIG_HEADER(config.h)
CFLAGS="-Wall -pedantic -funsigned-char"

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

# Checks for libraries.
check_popt=""
AC_CHECK_LIB([popt], [main], check_popt=true)
if test "x$check_popt" == x ; then
	AC_MSG_ERROR([*** libpopt not found.])
else
	POPT_LIBS=-lpopt
	AC_SUBST(POPT_LIBS)
	AC_DEFINE(HAVE_LIBPOPT,1,[Define if you have the POPT library])
fi


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([nl_langinfo setlocale strcasecmp strchr])


# AC_ARG_WITH(debug, [  --with-debug    Compile with debug information' ], CFLAGS="$CFLAGS -קףפבקיפר_תהוףר_זלבחי_הלס_ןפלבהכי")


AC_CONFIG_FILES([Makefile
				 libnatspec.pc
                 lib/Makefile
                 src/Makefile
				 tests/Makefile])

AC_OUTPUT
