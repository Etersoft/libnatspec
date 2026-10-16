/*
 * Public domain
 *
 * Known bugs in charset lookup by locale:
 * - locale with charset absent in the table falls back to the current
 *   charset instead of the charset from locale name and other
 *   charsets of the language
 * - charset aliases (windows-1251, CP866) are not recognized
 * - G_FILENAME_ENCODING is used (without check) for explicit locale too
 */

#include "check.h"
#include "natspec.h"

static void check_locale(const char *locale, int type, const char *expected)
{
	const char *got = natspec_get_charset_by_locale(type, locale);
	check_total++;
	if (got == NULL || strcmp(got, expected))
	{
		check_failed++;
		fprintf(stderr, "FAIL: charset_by_locale(%d, '%s'): got '%s', expected '%s'\n",
			type, locale, got ? got : "(null)", expected);
	}
}

static void test_charset_not_in_table(void)
{
	check_locale("be_BY.CP1251", NATSPEC_UNIXCS, "CP1251");
	check_locale("be_BY.CP1251", NATSPEC_DOSCS, "IBM866");
	check_locale("en_GB.ISO-8859-15", NATSPEC_UNIXCS, "ISO885915");
	check_locale("en_GB.ISO-8859-15", NATSPEC_WINCS, "CP1252");
	check_locale("de_DE.UTF-8@euro", NATSPEC_UNIXCS, "UTF8");
	check_locale("de_DE.UTF-8@euro", NATSPEC_DOSCS, "IBM850");
	check_locale("zh_CN.GB2312", NATSPEC_UNIXCS, "GB2312");
	check_locale("zh_TW.Big5", NATSPEC_UNIXCS, "BIG5");
	CHECK(natspec_locale_is_utf8("de_DE.UTF-8@euro"));
}

static void test_aliases(void)
{
	CHECK_INT(natspec_get_codepage_by_charset("windows-1251"), 1251);
	CHECK_INT(natspec_get_codepage_by_charset("CP866"), 866);
	CHECK_STR(natspec_get_nls_by_charset("windows-1251"), "cp1251");
	CHECK_STR(natspec_get_nls_by_charset("CP866"), "cp866");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_DOSCS, NATSPEC_WINCS, "windows-1251"), "IBM866");
}

static void test_filename_encoding_env(void)
{
	setenv("G_FILENAME_ENCODING", "KOI8-R", 1);
	CHECK_STR(natspec_get_filename_encoding("ru_RU.UTF-8"), "utf8");
	setenv("G_FILENAME_ENCODING", "NONEXISTENT", 1);
	CHECK_STR(natspec_get_filename_encoding("ru_RU.KOI8-R"), "koi8-r");
	unsetenv("G_FILENAME_ENCODING");
}

int main(void)
{
	unsetenv("LC_ALL");
	unsetenv("LC_CTYPE");
	unsetenv("LANG");
	test_charset_not_in_table();
	test_aliases();
	test_filename_encoding_env();
	return check_done();
}
