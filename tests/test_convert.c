/*
 * Public domain
 *
 * Regression tests for natspec_convert / natspec_iconv
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <assert.h>

#include "natspec.h"

/* U+2202 (PARTIAL DIFFERENTIAL) transliterates to "\partial" */
static void test_long_translit(void)
{
	char *in, *out;
	size_t i, n = 5000;
	in = malloc(n + 2);
	assert(in);
	for (i = 0; i < n; i++)
		in[i] = '\xB6'; /* PARTIAL DIFFERENTIAL in MACINTOSH */
	strcpy(in + n, "z");
	out = natspec_convert(in, "ASCII", "MACINTOSH", 1);
	assert(out);
	assert(strlen(out) == n * strlen("\\partial") + 1);
	assert(!strncmp(out, "\\partial\\partial", 16));
	assert(out[strlen(out) - 1] == 'z');
	free(out);
	free(in);
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
	assert(cd != (iconv_t)-1);
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 1);
	assert(res == (size_t)-1 && errno == E2BIG);
	assert(ol <= 4);
	assert(outp - out == 4 - (long)ol);
	assert(out[4] == '#');
	/* continue with enough space */
	ol = sizeof(out) - (outp - out);
	res = natspec_iconv(cd, &inp, &il, &outp, &ol, 1);
	assert(res != (size_t)-1 || errno == E2BIG);
	natspec_iconv_close(cd);
	assert(!strncmp(out, "abcZH", 5));
}

/* big input must not exhaust the stack */
static void test_big_input(void)
{
	size_t n = 20 * 1024 * 1024;
	char *in = malloc(n + 1), *out;
	assert(in);
	memset(in, 'a', n);
	in[n] = '\0';
	out = natspec_convert(in, "ASCII", "UTF-8", 1);
	assert(out);
	assert(strlen(out) == n);
	free(out);
	free(in);
}

int main(void)
{
	setlocale(LC_ALL, "");
	test_iconv_small_buffer();
	test_long_translit();
	test_big_input();
	printf("DONE\n");
	return 0;
}
