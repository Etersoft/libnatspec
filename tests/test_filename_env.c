/*
 * Public domain
 *
 * G_FILENAME_ENCODING in natspec_get_filename_encoding:
 * it is used for the current locale ("") only, the first charset
 * of comma separated list is used, unknown charset is ignored
 */

#include "check.h"
#include "natspec.h"

int main(void)
{
	unsetenv("LC_CTYPE");
	unsetenv("LANG");
	setenv("LC_ALL", "ru_RU.UTF-8", 1);

	setenv("G_FILENAME_ENCODING", "KOI8-R", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "koi8-r");
	/* explicit locale */
	CHECK_STR(natspec_get_filename_encoding("ru_RU.UTF-8"), "utf8");
	CHECK_STR(natspec_get_filename_encoding("uk_UA.CP1251"), "cp1251");

	/* unknown charset is ignored */
	setenv("G_FILENAME_ENCODING", "NONEXISTENT", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "utf8");
	CHECK_STR(natspec_get_filename_encoding("ru_RU.KOI8-R"), "koi8-r");

	/* first charset of the list */
	setenv("G_FILENAME_ENCODING", "CP1251,UTF-8", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "cp1251");
	setenv("G_FILENAME_ENCODING", "@locale,KOI8-R", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "utf8");

	unsetenv("G_FILENAME_ENCODING");
	unsetenv("LC_ALL");
	return check_done();
}
