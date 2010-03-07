dnl Configure paths for NATSPEC
dnl Vitaly Lipatov <lav@etersoft.ru>

dnl Tests for NATSPEC, and define NATSPEC_CFLAGS and NATSPEC_LIBS
dnl TODO: AM_PATH_NATSPEC([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
AC_DEFUN([AM_PATH_NATSPEC],
[dnl 
dnl Get the cflags and libraries from pkg-config
dnl

  with_natspec_support=no
  AC_ARG_WITH(natspec,
    [  --with-natspec          NATSPEC support (auto charset detecting)],
    [ case "$withval" in
      yes|no)
	    with_natspec_support="$withval"
	    ;;
    esac ])

  if test x$with_natspec_support != xno ; then
dnl    PKG_CHECK_MODULES(NATSPEC,libnatspec,with_natspec_support=yes,with_natspec_support=no)
dnl	AC_DEFINE(HAVE_NATSPEC,1,[Define if you have the natspec library]))
dnl	AM_CONDITIONAL(HAVE_NATSPEC, $with_natspec_support)

dnl	We need 0.2.4 due old ALT Linux systems. It is no needs to require latest version.

    AC_MSG_CHECKING([for NATSPEC support])
	PKG_CHECK_MODULES(NATSPEC, libnatspec >= 0.2.4,
		[AC_DEFINE(HAVE_NATSPEC,1,[Define if you have the natspec library])])
dnl	AC_MSG_RESULT(yes)
    AC_SUBST(NATSPEC_CFLAGS)
    AC_SUBST(NATSPEC_LIBS)
  fi

])

