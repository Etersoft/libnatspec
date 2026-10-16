/*
 * Public domain
 *
 * Tests for natspec_isutf8 (exported, but not declared in natspec.h)
 */

#include "check.h"

int natspec_isutf8(const char *buf);

int main(void)
{
	/* 7-bit text is not claimed as UTF-8 */
	CHECK_INT(natspec_isutf8(""), 0);
	CHECK_INT(natspec_isutf8("plain ascii text\n"), 0);
	/* valid UTF-8 */
	CHECK_INT(natspec_isutf8("\xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82"), 1);
	CHECK_INT(natspec_isutf8("caf\xC3\xA9\n"), 1);
	CHECK_INT(natspec_isutf8("\xE2\x88\x82"), 1);
	CHECK_INT(natspec_isutf8("\xF0\x9F\x98\x80"), 1);
	/* 8-bit encodings */
	CHECK_INT(natspec_isutf8("\xF0\xD2\xC9\xD7\xC5\xD4"), 0); /* KOI8-R */
	CHECK_INT(natspec_isutf8("\xCF\xF0\xE8\xE2\xE5\xF2"), 0); /* CP1251 */
	CHECK_INT(natspec_isutf8("caf\xE9"), 0); /* ISO-8859-1 */
	/* continuation byte without lead byte */
	CHECK_INT(natspec_isutf8("\x80"), 0);
	CHECK_INT(natspec_isutf8("\xC3\xA9\xA9"), 0);
	/* control characters are not text */
	CHECK_INT(natspec_isutf8("\xC3\xA9\x01"), 0);
	/* 0xFE/0xFF never appear in UTF-8 */
	CHECK_INT(natspec_isutf8("\xFE\xFF"), 0);
	return check_done();
}
