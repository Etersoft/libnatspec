AUTOMAKE_OPTIONS			= subdir-objects

AM_CFLAGS					= -I$(top_srcdir)/lib -pedantic
LDADD						= $(top_builddir)/lib/libnatspec.la

# lib/ sources are built with own CFLAGS to get separate object names
# lib/ sources without get_locale.c (it is replaced with i18n_get_locale.c)
LIB_NO_LOCALE_SOURCES		= ../lib/get_charset.c ../lib/enrich_fs_options.c \
	../lib/convert.c ../lib/filesystem.c ../lib/detect.c

PASS_TESTS					= test_get_charset test_names test_locale_charset test_env \
	test_convert

check_PROGRAMS				= $(PASS_TESTS)
TESTS						= $(PASS_TESTS)

EXTRA_DIST					= check.h test_util.h

test_env_SOURCES			= test_env.c i18n_get_locale.c $(LIB_NO_LOCALE_SOURCES)
test_env_LDADD				=
test_env_CFLAGS				= $(AM_CFLAGS)
