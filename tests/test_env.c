/*
 * Public domain
 *
 * Tests for locale detection from environment and system i18n file:
 * natspec_get_current_locale, natspec_get_system_locale,
 * natspec_get_charset_by_locale with "" and NULL, natspec_get_charset
 *
 * Linked with i18n_get_locale.c, the system i18n file is taken
 * from NATSPEC_TEST_I18N_FILE
 */

#include <unistd.h>
#include <locale.h>

#include "check.h"
#include "natspec.h"
#include "test_util.h"

static void clear_env(void)
{
	unsetenv("LC_ALL");
	unsetenv("LC_CTYPE");
	unsetenv("LANG");
	unsetenv("LANGUAGE");
	unsetenv("G_FILENAME_ENCODING");
	unsetenv("NATSPEC_TEST_I18N_FILE");
}

static void test_current_locale_env(void)
{
	clear_env();
	setenv("LANG", "ru_RU.KOI8-R", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.KOI8-R");
	setenv("LC_CTYPE", "uk_UA.UTF-8", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "uk_UA.UTF-8");
	setenv("LC_ALL", "en_US.UTF-8", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "en_US.UTF-8");
	/* empty value is skipped */
	setenv("LC_ALL", "", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "uk_UA.UTF-8");
	setenv("LC_CTYPE", "", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.KOI8-R");
	/* only first locale from list */
	setenv("LANG", "ru_RU.UTF-8:en_US", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.UTF-8");
	/* LANGUAGE is not used */
	setenv("LANGUAGE", "de_DE", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.UTF-8");
	CHECK_STR_FREE(natspec_get_user_locale(), "ru_RU.UTF-8");
	clear_env();
}

static void test_current_locale_fallback(void)
{
	char *file;
	clear_env();
	/* nothing at all */
	CHECK_STR_FREE(natspec_get_current_locale(), "C");
	CHECK_STR_FREE(natspec_get_system_locale(), "C");
	/* C and POSIX in environment mean system locale */
	file = test_write_file("LANG=ru_RU.KOI8-R\n");
	setenv("NATSPEC_TEST_I18N_FILE", file, 1);
	setenv("LANG", "C", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.KOI8-R");
	setenv("LANG", "POSIX", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "ru_RU.KOI8-R");
	setenv("LANG", "en_US", 1);
	CHECK_STR_FREE(natspec_get_current_locale(), "en_US");
	test_remove_file(file);
	clear_env();
}

static void check_i18n(const char *content, const char *expected)
{
	char *file = test_write_file(content);
	char *got;
	setenv("NATSPEC_TEST_I18N_FILE", file, 1);
	got = natspec_get_system_locale();
	check_total++;
	if (got == NULL || strcmp(got, expected))
	{
		check_failed++;
		fprintf(stderr, "FAIL: system locale for i18n file '%s': got '%s', expected '%s'\n",
			content, got ? got : "(null)", expected);
	}
	free(got);
	test_remove_file(file);
	unsetenv("NATSPEC_TEST_I18N_FILE");
}

static void test_system_locale(void)
{
	clear_env();
	check_i18n("LANG=ru_RU.KOI8-R\n", "ru_RU.KOI8-R");
	check_i18n("LANG=ru_RU.UTF-8", "ru_RU.UTF-8");
	check_i18n("LANG=\"ru_RU.UTF-8\"\n", "ru_RU.UTF-8");
	check_i18n("  LANG = ru_RU.UTF-8 \n", "ru_RU.UTF-8");
	check_i18n("LANG=ru_RU.UTF-8\r\n", "ru_RU.UTF-8");
	check_i18n("# comment\n\nSUPPORTED=ru_RU:en_US\nLANG=uk_UA.UTF-8\n", "uk_UA.UTF-8");
	check_i18n("LANG=uk_UA.UTF-8 # comment\n", "uk_UA.UTF-8");
	check_i18n("LANG=uk_UA.UTF-8;\n", "uk_UA.UTF-8");
	check_i18n("#LANG=en_US\nLANG=ru_RU\n", "ru_RU");
	check_i18n("LANG=ru_RU\nLANG=en_US\n", "ru_RU");
	/* no LANG in file: environment, then C */
	check_i18n("SYSFONT=cyr-sun16\n", "C");
	setenv("LANG", "be_BY.UTF-8", 1);
	check_i18n("SYSFONT=cyr-sun16\n", "be_BY.UTF-8");
	/* file has priority over environment */
	check_i18n("LANG=ru_RU.KOI8-R\n", "ru_RU.KOI8-R");
	clear_env();
}

static void test_charset_by_current_locale(void)
{
	char *file;
	clear_env();
	file = test_write_file("LANG=ru_RU.KOI8-R\n");
	setenv("NATSPEC_TEST_I18N_FILE", file, 1);
	setenv("LANG", "uk_UA.UTF-8", 1);
	/* "" is current locale */
	CHECK_STR(natspec_get_charset_by_locale(NATSPEC_UNIXCS, ""), "UTF8");
	CHECK_STR(natspec_get_charset_by_locale(NATSPEC_MACCS, ""), "MACUKRAINIAN");
	CHECK(natspec_locale_is_utf8(""));
	/* NULL is system locale */
	CHECK_STR(natspec_get_charset_by_locale(NATSPEC_UNIXCS, NULL), "KOI8R");
	CHECK_STR(natspec_get_charset_by_locale(NATSPEC_DOSCS, NULL), "IBM866");
	CHECK(!natspec_locale_is_utf8(NULL));
	CHECK_STR(natspec_get_filename_encoding(""), "utf8");
	CHECK_STR(natspec_get_filename_encoding(NULL), "koi8-r");
	test_remove_file(file);
	clear_env();
}

static void test_get_charset(void)
{
	clear_env();
	/* C locale */
	setlocale(LC_ALL, "C");
	CHECK_STR(natspec_get_charset(), "ANSI_X3.4-1968");
	if (setlocale(LC_ALL, "ru_RU.koi8r"))
		CHECK_STR(natspec_get_charset(), "KOI8R");
	else
		printf("skip: ru_RU.koi8r locale is not available\n");
	if (setlocale(LC_ALL, "ru_RU.utf8"))
		CHECK_STR(natspec_get_charset(), "UTF8");
	else
		printf("skip: ru_RU.utf8 locale is not available\n");
	setlocale(LC_ALL, "C");
}

int main(void)
{
	test_current_locale_env();
	test_current_locale_fallback();
	test_system_locale();
	test_charset_by_current_locale();
	test_get_charset();
	return check_done();
}
