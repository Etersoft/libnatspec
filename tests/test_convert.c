/*
 * Public domain
 *
 * Tests for natspec_convert / natspec_iconv
 */

#include <locale.h>
#include <errno.h>

#include "check.h"
#include "natspec.h"

/* "Привет" in various encodings */
#define HELLO_UTF8   "\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82"
#define HELLO_KOI8R  "\xF0\xD2\xC9\xD7\xC5\xD4"
#define HELLO_CP1251 "\xCF\xF0\xE8\xE2\xE5\xF2"
#define HELLO_CP866  "\x8F\xE0\xA8\xA2\xA5\xE2"

static void test_convert_basic(void)
{
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "KOI8-R", "UTF-8", 0), HELLO_KOI8R);
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "CP1251", "UTF-8", 0), HELLO_CP1251);
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "IBM866", "UTF-8", 0), HELLO_CP866);
	CHECK_STR_FREE(natspec_convert(HELLO_KOI8R, "UTF-8", "KOI8-R", 0), HELLO_UTF8);
	CHECK_STR_FREE(natspec_convert(HELLO_CP1251, "KOI8-R", "CP1251", 0), HELLO_KOI8R);
	CHECK_STR_FREE(natspec_convert(HELLO_CP866, "CP1251", "IBM866", 0), HELLO_CP1251);
	CHECK_STR_FREE(natspec_convert("plain ascii", "UTF-8", "ASCII", 0), "plain ascii");
	CHECK_STR_FREE(natspec_convert("", "KOI8-R", "UTF-8", 0), "");
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "UTF-8", "UTF-8", 0), HELLO_UTF8);
	/* obsolete alias */
	CHECK_STR_FREE(natspec_convert_with_translit(HELLO_UTF8, "ASCII", "UTF-8"), "Privet");
}

static void test_convert_locale_charset(void)
{
	/* NULL or empty charset means charset of the current locale */
	setlocale(LC_ALL, "C");
	CHECK_STR_FREE(natspec_convert("abc", NULL, "UTF-8", 0), "abc");
	CHECK_STR_FREE(natspec_convert("abc", "", "", 0), "abc");
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "", "UTF-8", 1), "Privet");
	if (setlocale(LC_ALL, "ru_RU.koi8r"))
	{
		CHECK_STR_FREE(natspec_convert(HELLO_UTF8, NULL, "UTF-8", 0), HELLO_KOI8R);
		CHECK_STR_FREE(natspec_convert(HELLO_KOI8R, "UTF-8", "", 0), HELLO_UTF8);
	}
	else
		printf("skip: ru_RU.koi8r locale is not available\n");
	setlocale(LC_ALL, "C");
}

static void test_convert_errors(void)
{
	/* unknown charset: original string is returned */
	CHECK_STR_FREE(natspec_convert("abc", "NONEXISTENT", "UTF-8", 0), "abc");
	CHECK_STR_FREE(natspec_convert("abc", "UTF-8", "NONEXISTENT", 1), "abc");
	CHECK_STR(natspec_convert(NULL, "UTF-8", "KOI8-R", 0), NULL);
	/* without transliteration conversion stops on invalid character */
	CHECK_STR_FREE(natspec_convert("ab" HELLO_UTF8, "ASCII", "UTF-8", 0), "ab");
}

static void test_translit(void)
{
	CHECK_STR_FREE(natspec_convert(HELLO_UTF8, "ASCII", "UTF-8", 1), "Privet");
	CHECK_STR_FREE(natspec_convert("x" HELLO_UTF8 "y", "ASCII", "UTF-8", 1), "xPrivety");
	/* U+0416 CYRILLIC CAPITAL LETTER ZHE */
	CHECK_STR_FREE(natspec_convert("\xD0\x96", "ASCII", "UTF-8", 1), "ZH");
	/* U+00E9 LATIN SMALL LETTER E WITH ACUTE */
	CHECK_STR_FREE(natspec_convert("caf\xC3\xA9", "ASCII", "UTF-8", 1), "cafe");
	/* U+00C4 LATIN CAPITAL LETTER A WITH DIAERESIS to KOI8-R */
	CHECK_STR_FREE(natspec_convert("\xC3\x84" HELLO_UTF8, "KOI8-R", "UTF-8", 1), "A:" HELLO_KOI8R);
	/* U+00A0 NO-BREAK SPACE */
	CHECK_STR_FREE(natspec_convert("a\xC2\xA0z", "ASCII", "UTF-8", 1), "a z");
	CHECK_STR_FREE(natspec_convert("a\xA0z", "ASCII", "ISO-8859-1", 1), "a z");
	/* U+00AD SOFT HYPHEN is dropped */
	CHECK_STR_FREE(natspec_convert("a\xC2\xADz", "ASCII", "UTF-8", 1), "az");
	/* character outside BMP gives one '_': U+1F600 GRINNING FACE */
	CHECK_STR_FREE(natspec_convert("a\xF0\x9F\x98\x80z", "ASCII", "UTF-8", 1), "a_z");
	CHECK_STR_FREE(natspec_convert("\xF0\x9F\x98\x80" HELLO_UTF8, "KOI8-R", "UTF-8", 1), "_" HELLO_KOI8R);
	/* U+2284 NOT A SUBSET OF */
	CHECK_STR_FREE(natspec_convert("\xE2\x8A\x84", "ASCII", "UTF-8", 1), "!(C");
	/* U+2202 PARTIAL DIFFERENTIAL is longer than 6 bytes */
	CHECK_STR_FREE(natspec_convert("\xE2\x88\x82", "ASCII", "UTF-8", 1), "\\partial");
	/* invalid UTF-8 byte is replaced with '_' */
	CHECK_STR_FREE(natspec_convert("a\xFF" "b", "ASCII", "UTF-8", 1), "a_b");
	/* character without transliteration: U+4E00 CJK */
	CHECK_STR_FREE(natspec_convert("a\xE4\xB8\x80" "b", "ASCII", "UTF-8", 1), "a_b");
	/* stateful source encoding: JIS X 0208 0x3021 in ISO-2022-JP */
	CHECK_STR_FREE(natspec_convert("a\x1B$B\x30\x21\x1B(Bz", "ASCII", "ISO-2022-JP", 1), "a_z");
}

static void test_long_translit(void)
{
	char *in, *out;
	size_t i, n = 5000;
	in = malloc(n + 2);
	if (!in)
		abort();
	for (i = 0; i < n; i++)
		in[i] = '\xB6'; /* PARTIAL DIFFERENTIAL in MACINTOSH */
	strcpy(in + n, "z");
	out = natspec_convert(in, "ASCII", "MACINTOSH", 1);
	CHECK(out != NULL);
	if (out)
	{
		CHECK_INT(strlen(out), n * strlen("\\partial") + 1);
		CHECK(!strncmp(out, "\\partial\\partial", 16));
		CHECK(out[strlen(out) - 1] == 'z');
	}
	free(out);
	free(in);
}

/* big input must not exhaust the stack */
static void test_big_input(void)
{
	size_t n = 20 * 1024 * 1024;
	char *in = malloc(n + 1), *out;
	if (!in)
		abort();
	memset(in, 'a', n);
	in[n] = '\0';
	out = natspec_convert(in, "ASCII", "UTF-8", 1);
	CHECK(out != NULL);
	if (out)
		CHECK_INT(strlen(out), n);
	free(out);
	free(in);
}

/* output with BOM and shift state */
static void test_convert_output_state(void)
{
	char *out;
	out = natspec_convert("a", "UTF-16", "ASCII", 0);
	CHECK(out && !memcmp(out, "\xFF\xFE" "a\0", 4));
	free(out);
	/* final ESC ( B returns to ASCII */
	CHECK_STR_FREE(natspec_convert("\xD0\x96", "ISO-2022-JP", "UTF-8", 0), "\x1B$B'(\x1B(B");
}

/* natspec_iconv must not write past the given output buffer */
static void test_iconv_small_buffer(void)
{
	char in[] = "abc\xD0\x96" "AAAA"; /* U+0416 transliterates to "ZH" */
	char out[8];
	char *inp = in, *outp = out;
	size_t il = strlen(in), ol = 4;
	size_t res;
	iconv_t cd;

	memset(out, '#', sizeof(out));
	cd = natspec_iconv_open("ASCII", "UTF-8");
	CHECK(cd != (iconv_t)-1);
	if (cd == (iconv_t)-1)
		return;
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 1);
	CHECK(res == (size_t)-1 && errno == E2BIG);
	CHECK_INT(ol, 1);
	CHECK_INT(outp - out, 3);
	CHECK_INT(inp - in, 3);
	CHECK(out[3] == '#');
	/* continue with enough space */
	ol = sizeof(out) - (outp - out);
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 1);
	CHECK(res == (size_t)-1 && errno == E2BIG);
	CHECK(!strncmp(out, "abcZHAAA", 8));
	natspec_iconv_close(cd);
}

static void test_iconv(void)
{
	char in[] = "a" HELLO_UTF8 "\xFF" "z";
	char out[64];
	char *inp = in, *outp = out;
	size_t il = strlen(in), ol = sizeof(out) - 1;
	size_t res;
	iconv_t cd = natspec_iconv_open("KOI8-R", "UTF-8");
	CHECK(cd != (iconv_t)-1);
	if (cd == (iconv_t)-1)
		return;
	/* without transliteration it stops like iconv */
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 0);
	CHECK(res == (size_t)-1 && errno == EILSEQ);
	CHECK_INT(inp - in, 1 + strlen(HELLO_UTF8));
	/* with transliteration it skips invalid byte */
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 1);
	CHECK(res != (size_t)-1);
	CHECK_INT(il, 0);
	*outp = '\0';
	CHECK_STR(out, "a" HELLO_KOI8R "_z");
	CHECK_INT(natspec_iconv_close(cd), 0);

	CHECK(natspec_iconv_open("NONEXISTENT", "UTF-8") == (iconv_t)-1);
	CHECK(natspec_iconv_open("UTF-8", "NONEXISTENT") == (iconv_t)-1);
}

int main(void)
{
	setlocale(LC_ALL, "C");
	test_convert_basic();
	test_convert_locale_charset();
	test_convert_errors();
	test_translit();
	test_long_translit();
	test_big_input();
	test_convert_output_state();
	test_iconv_small_buffer();
	test_iconv();
	return check_done();
}
