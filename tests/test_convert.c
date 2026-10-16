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

int main(void)
{
	setlocale(LC_ALL, "");
	test_iconv_small_buffer();
	printf("DONE\n");
	return 0;
}
