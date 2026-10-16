/*
 * Public domain
 *
 * Tests for charset/locale name helpers:
 * natspec_humble_charset, natspec_extract_charset_from_locale,
 * _natspec_repack_locale, natspec_get_codepage_by_charset,
 * natspec_get_nls_by_charset
 */

#include "check.h"
#include "natspec.h"

/* internal, but exported */
char *_natspec_repack_locale(const char *locale);

static void test_humble_charset(void)
{
	CHECK_STR(natspec_humble_charset(NULL), NULL);
	CHECK_STR(natspec_humble_charset(""), NULL);
	CHECK_STR_FREE(natspec_humble_charset("KOI8-R"), "KOI8R");
	CHECK_STR_FREE(natspec_humble_charset("koi8r"), "KOI8R");
	CHECK_STR_FREE(natspec_humble_charset("utf-8"), "UTF8");
	CHECK_STR_FREE(natspec_humble_charset("ISO-8859-15"), "ISO885915");
	CHECK_STR_FREE(natspec_humble_charset("iso_8859-1"), "ISO88591");
	CHECK_STR_FREE(natspec_humble_charset("cp1251"), "CP1251");
	/* everything after ':' is dropped */
	CHECK_STR_FREE(natspec_humble_charset("KOI8-R:UTF-8"), "KOI8R");
	CHECK_STR_FREE(natspec_humble_charset(":UTF-8"), "");
	/* ANSI_X3.4-1968 (ASCII in glibc) is kept as is */
	CHECK_STR_FREE(natspec_humble_charset("ANSI_X3.4-1968"), "ANSI_X3.4-1968");
	CHECK_STR_FREE(natspec_humble_charset("--"), "");
	/* aliases */
	CHECK_STR_FREE(natspec_humble_charset("windows-1251"), "CP1251");
	CHECK_STR_FREE(natspec_humble_charset("CP866"), "IBM866");
	CHECK_STR_FREE(natspec_humble_charset("cp437"), "IBM437");
	CHECK_STR_FREE(natspec_humble_charset("macintosh"), "MAC");
}

static void test_extract_charset(void)
{
	CHECK_STR(natspec_extract_charset_from_locale(NULL), NULL);
	CHECK_STR(natspec_extract_charset_from_locale(""), NULL);
	CHECK_STR(natspec_extract_charset_from_locale("ru_RU"), NULL);
	CHECK_STR(natspec_extract_charset_from_locale("de_DE@euro"), NULL);
	CHECK_STR_FREE(natspec_extract_charset_from_locale("ru_RU.KOI8-R"), "KOI8R");
	CHECK_STR_FREE(natspec_extract_charset_from_locale("ru_RU.utf8"), "UTF8");
	CHECK_STR_FREE(natspec_extract_charset_from_locale("de_DE.ISO-8859-15@euro"), "ISO885915");
	CHECK_STR_FREE(natspec_extract_charset_from_locale("ru_RU.KOI8-R:en_US.UTF-8"), "KOI8R");
	CHECK_STR_FREE(natspec_extract_charset_from_locale("C.UTF-8"), "UTF8");
}

static void test_repack_locale(void)
{
	CHECK_STR(_natspec_repack_locale(NULL), NULL);
	CHECK_STR(_natspec_repack_locale(""), NULL);
	CHECK_STR_FREE(_natspec_repack_locale("ru_RU"), "ru_RU");
	CHECK_STR_FREE(_natspec_repack_locale("ru"), "ru");
	CHECK_STR_FREE(_natspec_repack_locale("ru_RU.KOI8-R"), "ru_RU.KOI8R");
	CHECK_STR_FREE(_natspec_repack_locale("ru_RU.utf8"), "ru_RU.UTF8");
	CHECK_STR_FREE(_natspec_repack_locale("RU_ru.koi8-r"), "ru_RU.KOI8R");
	CHECK_STR_FREE(_natspec_repack_locale("de_DE@EURO"), "de_DE@euro");
	CHECK_STR_FREE(_natspec_repack_locale("de_DE.UTF-8@euro"), "de_DE.UTF8@euro");
	CHECK_STR_FREE(_natspec_repack_locale("ru_RU.KOI8-R:en_US"), "ru_RU.KOI8R");
	CHECK_STR_FREE(_natspec_repack_locale("sr_RS@latin"), "sr_RS@latin");
	/* C and POSIX are case sensitive */
	CHECK_STR_FREE(_natspec_repack_locale("C"), "C");
	CHECK_STR_FREE(_natspec_repack_locale("POSIX"), "POSIX");
	CHECK_STR_FREE(_natspec_repack_locale("C.UTF-8"), "C.UTF8");
}

static void test_codepage(void)
{
	CHECK_INT(natspec_get_codepage_by_charset("IBM866"), 866);
	CHECK_INT(natspec_get_codepage_by_charset("ibm-866"), 866);
	CHECK_INT(natspec_get_codepage_by_charset("CP1251"), 1251);
	CHECK_INT(natspec_get_codepage_by_charset("cp1252"), 1252);
	CHECK_INT(natspec_get_codepage_by_charset("IBM850"), 850);
	CHECK_INT(natspec_get_codepage_by_charset("CP1125"), 1125);
	CHECK_INT(natspec_get_codepage_by_charset("KOI8-R"), 20866);
	CHECK_INT(natspec_get_codepage_by_charset("KOI8-U"), 21866);
	CHECK_INT(natspec_get_codepage_by_charset("UTF-8"), 65001);
	CHECK_INT(natspec_get_codepage_by_charset("ISO-8859-1"), 28591);
	CHECK_INT(natspec_get_codepage_by_charset("ISO-8859-5"), 28595);
	CHECK_INT(natspec_get_codepage_by_charset("GB2312"), 936);
	CHECK_INT(natspec_get_codepage_by_charset("BIG5"), 950);
	CHECK_INT(natspec_get_codepage_by_charset("windows-1251"), 1251);
	CHECK_INT(natspec_get_codepage_by_charset("CP866"), 866);
	/* unknown charset gives ASCII codepage */
	CHECK_INT(natspec_get_codepage_by_charset("NONEXISTENT"), 437);
}

static void test_nls(void)
{
	CHECK_STR(natspec_get_nls_by_charset("KOI8-R"), "koi8-r");
	CHECK_STR(natspec_get_nls_by_charset("koi8u"), "koi8-u");
	CHECK_STR(natspec_get_nls_by_charset("UTF-8"), "utf8");
	CHECK_STR(natspec_get_nls_by_charset("CP1251"), "cp1251");
	CHECK_STR(natspec_get_nls_by_charset("IBM866"), "cp866");
	CHECK_STR(natspec_get_nls_by_charset("IBM437"), "cp437");
	CHECK_STR(natspec_get_nls_by_charset("ISO-8859-1"), "iso8859-1");
	CHECK_STR(natspec_get_nls_by_charset("ISO8859-15"), "iso8859-15");
	CHECK_STR(natspec_get_nls_by_charset("EUC-JP"), "euc-jp");
	CHECK_STR(natspec_get_nls_by_charset("GB2312"), "gb2312");
	CHECK_STR(natspec_get_nls_by_charset("windows-1251"), "cp1251");
	CHECK_STR(natspec_get_nls_by_charset("CP866"), "cp866");
	/* fallback is always a valid nls */
	CHECK_STR(natspec_get_nls_by_charset(NULL), "iso8859-1");
	CHECK_STR(natspec_get_nls_by_charset("NONEXISTENT"), "iso8859-1");
}

int main(void)
{
	test_humble_charset();
	test_extract_charset();
	test_repack_locale();
	test_codepage();
	test_nls();
	return check_done();
}
