# Configure ac for LIBNATSPEC project
#
AC_PREREQ(2.52)
# TODO: get version from spec file
# TODO: add libtool versioning current:revision:age
AC_INIT([libnatspec], [0.4.0], lav@etersoft.ru)
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])
CFLAGS="$CFLAGS -Wall -funsigned-char"

dnl Initialize maintainer mode
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
#AC_PROG_LIBTOOL

dnl Initialize libtool
LT_INIT([disable-static])
AM_PROG_CC_C_O

dnl Checks for libraries.
check_popt=""
AC_CHECK_LIB([popt], [poptGetContext], check_popt=true)
if test "x$check_popt" = x ; then
	AC_MSG_WARN([ libpopt not found.])
else
	POPT_LIBS=-lpopt
	AC_SUBST(POPT_LIBS)
	AC_DEFINE(HAVE_LIBPOPT,1,[Define if you have the POPT library])
fi

dnl pthread is in libc since glibc 2.34
AC_SEARCH_LIBS([pthread_mutex_lock], [pthread])

dnl Checks for header files.
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h strings.h])

check_iconv=""
AC_CHECK_HEADERS([iconv.h],check_iconv=true)
if test "x$check_iconv" = x ; then
	AC_MSG_ERROR([*** iconv.h not found.])
fi

dnl iconv is in libc with glibc, GNU libiconv needs -liconv
dnl (its iconv.h renames iconv_open to libiconv_open)
ICONV_LIBS=""
AC_MSG_CHECKING([for library with iconv_open])
AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <iconv.h>]],
		[[iconv_t cd = iconv_open("", ""); iconv_close(cd);]])],
	[AC_MSG_RESULT([none required])],
	[save_LIBS="$LIBS"
	LIBS="$LIBS -liconv"
	AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <iconv.h>]],
			[[iconv_t cd = iconv_open("", ""); iconv_close(cd);]])],
		[AC_MSG_RESULT([-liconv])
		ICONV_LIBS="-liconv"],
		[AC_MSG_RESULT([no])
		AC_MSG_ERROR([*** iconv_open not found.])])])
AC_SUBST(ICONV_LIBS)

# check Doxygen
DOXYGEN=""
AC_ARG_WITH(doxygen, [  --with-doxygen    Generate doxygen documentation ],
[], [with_doxygen=no])
AS_IF([test "x$with_doxygen" != xno],
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
AC_CHECK_FUNCS([nl_langinfo setlocale strcasecmp strchr strdup strstr])

dnl Python
AC_ARG_WITH(python, [  --with-python    Compile python binding ],
[], [with_python=no])
AS_IF([test "x$with_python" != xno],
  [AM_PATH_PYTHON(2.3)
  AM_CHECK_PYTHON_HEADERS(HAVE_PYTHON=yes,[HAVE_PYTHON=no;AC_MSG_WARN([python headers not found, disabling python binding])])
])
AM_CONDITIONAL(HAVE_PYTHON, [test "x$HAVE_PYTHON" = xyes])

dnl #AS_COMPILER_FLAG([-fno-strict-aliasing],[PYTHON_QUIET="$PYTHON_QUIET -fno-strict-aliasing"], true )
dnl #AS_COMPILER_FLAG([-Wno-unused-function],[PYTHON_QUIET="$PYTHON_QUIET -Wno-unused-function"], true )
dnl #AC_SUBST(PYTHON_QUIET)


AC_ARG_WITH(debug, [  --with-debug    Compile with debug information],
[], [with_debug=no])
AS_IF([test "x$with_debug" != xno],
[CFLAGS="$CFLAGS -g"])

AC_ARG_WITH(i18n-file, [  --with-i18n-file=file Use this file (or ':' separated list of files)
                        for system LANG search [/etc/locale.conf:/etc/sysconfig/i18n]],
	i18n_file=$withval,
	i18n_file=/etc/locale.conf:/etc/sysconfig/i18n)

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
