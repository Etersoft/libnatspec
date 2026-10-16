/*
 * Public domain
 *
 * Tests for charset lookup by explicit locale name:
 * natspec_get_charset_by_locale, natspec_locale_is_utf8,
 * natspec_get_filename_encoding, natspec_get_charset_by_charset
 *
 * setlocale is not called, so the fallback charset is ANSI_X3.4-1968
 */

#include "check.h"
#include "natspec.h"

#define ASCII "ANSI_X3.4-1968"

static void check_locale(const char *locale, const char *unix_cs,
	const char *win_cs, const char *dos_cs, const char *mac_cs)
{
	const char *got[4];
	const char *expected[4];
	int i;
	expected[NATSPEC_UNIXCS] = unix_cs;
	expected[NATSPEC_WINCS] = win_cs;
	expected[NATSPEC_DOSCS] = dos_cs;
	expected[NATSPEC_MACCS] = mac_cs;
	for (i = NATSPEC_UNIXCS; i <= NATSPEC_MACCS; i++)
	{
		got[i] = natspec_get_charset_by_locale(i, locale);
		check_total++;
		if (got[i] == NULL || strcmp(got[i], expected[i]))
		{
			check_failed++;
			fprintf(stderr, "FAIL: charset_by_locale(%d, '%s'): got '%s', expected '%s'\n",
				i, locale, got[i] ? got[i] : "(null)", expected[i]);
		}
	}
}

static void test_charset_by_locale(void)
{
	check_locale("ru_RU", "ISO88595", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.KOI8-R", "KOI8R", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.koi8r", "KOI8R", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.UTF-8", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.utf8", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.CP1251", "CP1251", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.cp866", "IBM866", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.windows-1251", "CP1251", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("RU_ru.KOI8-R", "KOI8R", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.KOI8-R:en_US", "KOI8R", "CP1251", "IBM866", "MACCYRILLIC");
	/* Windows uses OEM 866 for Ukrainian, Linux kernel has no cp1125 nls */
	check_locale("uk_UA", "KOI8U", "CP1251", "IBM866", "MACUKRAINIAN");
	check_locale("uk_UA.UTF-8", "UTF8", "CP1251", "IBM866", "MACUKRAINIAN");
	check_locale("be_BY", "CP1251", "CP1251", "IBM866", "MACCYRILLIC");
	/* lib/data/charset_overrides.txt */
	check_locale("kk_KZ", "PT154", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("kk_KZ.UTF-8", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("cv_RU", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("mhr_RU", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("kv_RU.UTF-8", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("os_RU", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("en_US", "ISO88591", "CP1252", "IBM437", "MAC");
	check_locale("en_US.UTF-8", "UTF8", "CP1252", "IBM437", "MAC");
	check_locale("de_DE@euro", "ISO885915", "CP1252", "IBM850", "MAC");
	check_locale("ja_JP.EUC-JP", "EUCJP", "CP932", "CP932", "MAC");
	check_locale("ko_KR.EUC-KR", "EUCKR", "CP949", "CP949", "MAC");
	check_locale("tr_TR", "ISO88599", "CP1254", "IBM857", "MAC");
	check_locale("tr_TR.UTF-8", "UTF8", "CP1254", "IBM857", "MAC");
	check_locale("el_GR", "ISO88597", "CP1253", "CP437", "MAC");
	check_locale("he_IL", "ISO88598", "CP1255", "IBM862", "MAC");
	check_locale("pl_PL", "ISO88592", "CP1250", "IBM852", "MAC");
	check_locale("th_TH", "TIS620", "IBM874", "IBM874", "MAC");
	check_locale("C", ASCII, "CP1252", "IBM437", "MAC");
	check_locale("POSIX", ASCII, "CP1252", "IBM437", "MAC");
	check_locale("C.UTF-8", "UTF8", "CP1252", "IBM437", "MAC");
	check_locale("C.utf8", "UTF8", "CP1252", "IBM437", "MAC");
	/* charset of locale is not in the table: charset from the name,
	   other charsets of the language */
	check_locale("be_BY.CP1251", "CP1251", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("en_GB.ISO-8859-15", "ISO885915", "CP1252", "IBM850", "MAC");
	check_locale("de_DE.UTF-8@euro", "UTF8", "CP1252", "IBM850", "MAC");
	check_locale("zh_CN.GB2312", "GB2312", "CP936", "CP936", "MAC");
	check_locale("zh_TW.Big5", "BIG5", "BIG5", "BIG5", "MAC");
	check_locale("uz_UZ.UTF-8@cyrillic", "UTF8", "CP1251", "IBM866", "MACCYRILLIC");
	check_locale("ru_RU.KOI8-R@nonexistent", "KOI8R", "CP1251", "IBM866", "MACCYRILLIC");
	/* country is unknown, but charset is known */
	check_locale("xx_YY.UTF-8", "UTF8", ASCII, ASCII, ASCII);
	/* unknown charset */
	check_locale("ru_RU.NONEXISTENT", ASCII, ASCII, ASCII, ASCII);
	/* unknown locale falls back to current charset */
	check_locale("xx_YY", ASCII, ASCII, ASCII, ASCII);
}

static void test_locale_is_utf8(void)
{
	CHECK(natspec_locale_is_utf8("ru_RU.UTF-8"));
	CHECK(natspec_locale_is_utf8("ru_RU.utf8"));
	CHECK(natspec_locale_is_utf8("en_US.UTF-8"));
	CHECK(natspec_locale_is_utf8("C.UTF-8"));
	CHECK(!natspec_locale_is_utf8("C"));
	CHECK(!natspec_locale_is_utf8("ru_RU.KOI8-R"));
	CHECK(!natspec_locale_is_utf8("ru_RU"));
	CHECK(!natspec_locale_is_utf8("en_US"));
	CHECK(!natspec_locale_is_utf8("xx_YY"));
	CHECK(natspec_locale_is_utf8("de_DE.UTF-8@euro"));
	CHECK(natspec_locale_is_utf8("xx_YY.utf8"));
}

static void test_filename_encoding(void)
{
	unsetenv("G_FILENAME_ENCODING");
	CHECK_STR(natspec_get_filename_encoding("ru_RU.KOI8-R"), "koi8-r");
	CHECK_STR(natspec_get_filename_encoding("ru_RU.UTF-8"), "utf8");
	CHECK_STR(natspec_get_filename_encoding("ru_RU"), "iso8859-5");
	CHECK_STR(natspec_get_filename_encoding("ru_RU.CP1251"), "cp1251");
	CHECK_STR(natspec_get_filename_encoding("uk_UA"), "koi8-u");
	CHECK_STR(natspec_get_filename_encoding("en_US"), "iso8859-1");
	CHECK_STR(natspec_get_filename_encoding("de_DE@euro"), "iso8859-15");
	CHECK_STR(natspec_get_filename_encoding("ja_JP.EUC-JP"), "euc-jp");
	CHECK_STR(natspec_get_filename_encoding("xx_YY"), "iso8859-1");

	/* G_FILENAME_ENCODING is used for current locale (""),
	   for explicit locale it is checked in xfail_locale_lookup */
	setenv("G_FILENAME_ENCODING", "KOI8-R", 1);
	setenv("LC_ALL", "ru_RU.UTF-8", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "koi8-r");
	/* @locale means charset of the current locale */
	setenv("G_FILENAME_ENCODING", "@locale", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "utf8");
	/* unknown charset in G_FILENAME_ENCODING is ignored */
	setenv("G_FILENAME_ENCODING", "NONEXISTENT", 1);
	CHECK_STR(natspec_get_filename_encoding(""), "utf8");
	unsetenv("G_FILENAME_ENCODING");
	unsetenv("LC_ALL");
}

static void test_charset_by_charset(void)
{
	/* by unix charset */
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_UNIXCS, "KOI8-R"), "CP1251");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_DOSCS, NATSPEC_UNIXCS, "koi8r"), "IBM866");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_MACCS, NATSPEC_UNIXCS, "KOI8-R"), "MACCYRILLIC");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_UNIXCS, NATSPEC_UNIXCS, "KOI8-R"), "KOI8R");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_UNIXCS, "ISO-8859-5"), "CP1251");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_UNIXCS, "EUC-JP"), "CP932");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_UNIXCS, NATSPEC_UNIXCS, "UTF-8"), "UTF8");
	/* by windows charset */
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_DOSCS, NATSPEC_WINCS, "windows-1251"), "IBM866");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_DOSCS, NATSPEC_WINCS, "CP1251"), "IBM866");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_WINCS, "cp1251"), "CP1251");
	/* ASCII aliases (charset_alias_relation) */
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_UNIXCS, "ASCII"), "CP1252");
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_DOSCS, NATSPEC_UNIXCS, "ANSI_X3.4-1968"), "IBM437");
	/* unknown */
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_WINCS, NATSPEC_UNIXCS, "NONEXISTENT"), NULL);
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_UNIXCS, NATSPEC_WINCS, "KOI8-R"), NULL);
	CHECK_STR(natspec_get_charset_by_charset(NATSPEC_UNIXCS, NATSPEC_WINCS, NULL), NULL);
}

int main(void)
{
	unsetenv("LC_ALL");
	unsetenv("LC_CTYPE");
	unsetenv("LANG");
	unsetenv("G_FILENAME_ENCODING");
	test_charset_by_locale();
	test_locale_is_utf8();
	test_filename_encoding();
	test_charset_by_charset();
	return check_done();
}
