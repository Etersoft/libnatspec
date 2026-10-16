/*
 * Public domain
 *
 * Known bugs in charset lookup by locale:
 * - locale with charset absent in the table falls back to the current
 *   charset instead of the charset from locale name and other
 *   charsets of the language
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
	test_filename_encoding_env();
	return check_done();
}
