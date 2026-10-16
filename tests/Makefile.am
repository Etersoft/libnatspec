AUTOMAKE_OPTIONS			= subdir-objects

AM_CFLAGS					= -I$(top_srcdir)/lib -pedantic
LDADD						= $(top_builddir)/lib/libnatspec.la

PASS_TESTS					= test_get_charset test_names test_convert

check_PROGRAMS				= $(PASS_TESTS)
TESTS						= $(PASS_TESTS)

EXTRA_DIST					= check.h
