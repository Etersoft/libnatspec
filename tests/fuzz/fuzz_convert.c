/*
 * Public domain
 *
 * libFuzzer harness: natspec_convert() and natspec_iconv()
 * with transliteration for various charsets and output buffer sizes.
 * Input: byte 0 - source charset, byte 1 - target charset,
 *        byte 2 - flags (bit 0: transliterate) and output buffer size,
 *        rest - the string.
 */

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <iconv.h>

#include "natspec.h"

static const char *charsets[] = {
	"UTF-8", "ASCII", "KOI8-R", "CP1251", "IBM866", "ISO-8859-5", "MACINTOSH",
	"MAC-CYRILLIC", "ISO-2022-JP", "UTF-16", "UCS-2", "GB18030", "BIG5-HKSCS",
	"EUC-JP", "SJIS", "UTF-7", "IEC_P27-1", "", NULL /* NULL = current */
};
#define NCS (sizeof(charsets) / sizeof(charsets[0]))

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
	const char *from, *to;
	char *in, *out;
	int translit;
	size_t outsize;

	if (size < 3)
		return 0;
	from = charsets[data[0] % NCS];
	to = charsets[data[1] % NCS];
	translit = data[2] & 1;
	outsize = data[2] >> 1; /* 0..127 */
	data += 3; size -= 3;

	in = malloc(size + 1);
	memcpy(in, data, size);
	in[size] = '\0';

	out = natspec_convert(in, to, from, translit);
	free(out);

	{
		/* exact size output buffer: ASan reports any write past it */
		iconv_t cd = natspec_iconv_open(to, from);
		if (cd != (iconv_t)-1)
		{
			char *buf = malloc(outsize ? outsize : 1);
			char *inp = in, *outp = buf;
			size_t il = strlen(in), ol = outsize;
			natspec_iconv(cd, &inp, &il, &outp, &ol, translit);
			if (ol > outsize || (size_t)(outp - buf) + ol != outsize)
				abort(); /* wrong output accounting */
			free(buf);
			natspec_iconv_close(cd);
		}
	}
	free(in);
	return 0;
}
