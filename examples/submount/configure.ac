AC_INIT(submountd,0.9,<eweiss@sbcglobal.net>)
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])
AC_PROG_CC
AC_PROG_INSTALL
AC_GNU_SOURCE
CFLAGS="-O2 -Wall"

dnl
dnl check for resmgr with rsm_list_sessions() ( SuSE 9.1 and newer )
dnl
AC_ARG_ENABLE(resmgr, [  --disable-resmgr     don't require libresmgr ],  with_resmgr=$enableval, with_resmgr=yes)
if test "$with_resmgr" = "yes"; then
AC_MSG_CHECKING(for resmgr support)
save_resmgrtest_LIBS="$LIBS"
save_resmgrtest_LDFLAGS="$LDFLAGS"
save_resmgrtest_CPPFLAGS="$CPPFLAGS"
LDFLAGS="$all_libraries $LDFLAGS"
CFLAGS="$CFLAGS $all_includes"
LIBS="-lresmgr"
AC_TRY_LINK(	[
			#include <resmgr.h>
		],[
			rsm_list_sessions();
		],[
			AC_DEFINE(USE_RESMGR,1,[Define if resmgr is available])
			LIB_RESMGR="-lresmgr"
			AC_MSG_RESULT(yes)
		],[
			AC_MSG_RESULT(no)
			LIB_RESMGR=""
])
CFLAGS=$save_slptest_CPPFLAGS
LDFLAGS=$save_slptest_LDFLAGS
LIBS=$save_slptest_LIBS
fi
AC_SUBST(LIB_RESMGR)

# Checks for libnatspec.
#PKG_CHECK_MODULES(NATSPEC, libnatspec,,exit)
#AC_SUBST(NATSPEC_LIBS)
#AC_SUBST(NATSPEC_CFLAGS)

check_natspec=""
AC_CHECK_LIB([natspec], [main], check_natspec=true)
if test ! "x$check_natspec" == x ; then
NATSPEC_LIBS=-lnatspec
AC_SUBST(NATSPEC_LIBS)
AC_DEFINE(HAVE_natspec,1,[Define if you have the natspec library])]
fi
			
AC_PREFIX_DEFAULT(/)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
