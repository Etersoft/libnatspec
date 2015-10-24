# Configure ac for LIBNATSPEC project
#
AC_PREREQ(2.52)
# TODO: get version from spec file
# TODO: add libtool versioning current:revision:age
AC_INIT([libnatspec], [0.3.1], lav@etersoft.ru)
dnl AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME,AC_PACKAGE_VERSION)
dnl AS_VERSION(comedilib, COMEDILIB, 0, 7, 21)
dnl AS_LIBTOOL(COMEDILIB, 0, 7, 18, yes)

AC_CONFIG_HEADER(config.h)
CFLAGS="$CFLAGS -Wall -funsigned-char"

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
AM_PROG_CC_C_O

dnl Checks for libraries.
check_popt=""
AC_CHECK_LIB([popt], [main], check_popt=true)
if test "x$check_popt" == x ; then
	AC_MSG_WARN([ libpopt not found.])
else
	POPT_LIBS=-lpopt
	AC_SUBST(POPT_LIBS)
	AC_DEFINE(HAVE_LIBPOPT,1,[Define if you have the POPT library])
fi

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h strings.h])

check_iconv=""
AC_CHECK_HEADERS([iconv.h],check_iconv=true)
if test "x$check_iconv" == x ; then
	AC_MSG_ERROR([*** iconv.h not found.])
fi

# check Doxygen
DOXYGEN=""
AC_ARG_WITH(doxygen, [  --with-doxygen    Generate doxygen documentation ],
[AC_CHECK_PROG(DOX, doxygen, yes)
if test "x$DOX" != x ; then
	DOXYGEN="doxygen"
else
	AC_MSG_ERROR([*** Doxygen not found])
fi
])
AC_SUBST(DOXYGEN)
AM_CONDITIONAL(HAVE_DOXYGEN, test x$DOXYGEN != x)



dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([nl_langinfo setlocale strcasecmp strchr strdup strstr])

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

AC_ARG_WITH(i18n-file, [  --with-i18n-file=file Use this file for system LANG search],
	i18n_file=$withval,
	i18n_file=/etc/sysconfig/i18n)

AC_DEFINE_UNQUOTED(NATSPEC_I18N_FILE,"$i18n_file",[ File with system wide LANG variable])
dnl case "$target_os" in
dnl AC_MSG_WARN([We have target $target_os])

AC_CONFIG_FILES([Makefile
                 libnatspec.pc
                 natspec.m4
                 lib/Makefile
                 libhi/Makefile
                 src/Makefile
                 src/python/Makefile
                 docs/libnatspecDox.cfg
                 docs/Makefile
                 tests/Makefile])

AC_OUTPUT
