AUTOMAKE_OPTIONS			= subdir-objects

AM_CFLAGS					= -I$(top_srcdir)/lib -pedantic
LDADD						= $(top_builddir)/lib/libnatspec.la

# lib/ sources are built with own CFLAGS to get separate object names
# lib/ sources without get_locale.c (it is replaced with i18n_get_locale.c)
LIB_NO_LOCALE_SOURCES		= ../lib/get_charset.c ../lib/enrich_fs_options.c \
	../lib/convert.c ../lib/filesystem.c ../lib/detect.c

PASS_TESTS					= test_get_charset test_names test_locale_charset test_env \
	test_enrich test_convert test_detect test_tables test_iconv_open \
	test_null_charset

# programs for known bugs, move to PASS_TESTS after fix
XFAIL_TESTS					= xfail_tables xfail_locale_lookup \
	xfail_i18n xfail_translit

check_PROGRAMS				= $(PASS_TESTS) $(XFAIL_TESTS)
dist_check_SCRIPTS			= test_cli.sh test_python.sh
TESTS						= $(PASS_TESTS) $(XFAIL_TESTS) test_cli.sh
if HAVE_PYTHON
TESTS						+= test_python.sh
endif

AM_TESTS_ENVIRONMENT		= top_builddir='$(top_builddir)'; top_srcdir='$(top_srcdir)'; \
	PYTHON='$(PYTHON)'; export top_builddir top_srcdir PYTHON;

EXTRA_DIST					= check.h test_util.h

test_env_SOURCES			= test_env.c i18n_get_locale.c $(LIB_NO_LOCALE_SOURCES)
test_env_LDADD				=
test_env_CFLAGS				= $(AM_CFLAGS)
xfail_i18n_SOURCES			= xfail_i18n.c i18n_get_locale.c $(LIB_NO_LOCALE_SOURCES)
xfail_i18n_LDADD			=
xfail_i18n_CFLAGS			= $(AM_CFLAGS)
# use tables directly, without the library
test_tables_LDADD			=
xfail_tables_SOURCES		= xfail_tables.c ../lib/get_locale.c ../lib/get_charset.c
xfail_tables_LDADD			=
xfail_tables_CFLAGS			= $(AM_CFLAGS)
test_iconv_open_LDADD		= $(LDADD) -lpthread
