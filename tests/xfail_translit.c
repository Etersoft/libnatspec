/*
 * Public domain
 *
 * Known bugs in transliteration data and logic:
 * - U+00A0 NO-BREAK SPACE gives control character \001
 * - character outside BMP gives one '_' per byte
 */

#include "check.h"
#include "natspec.h"

int main(void)
{
	CHECK_STR_FREE(natspec_convert("a\xC2\xA0z", "ASCII", "UTF-8", 1), "a z");
	CHECK_STR_FREE(natspec_convert("a\xA0z", "ASCII", "ISO-8859-1", 1), "a z");
	/* U+1F600 GRINNING FACE */
	CHECK_STR_FREE(natspec_convert("a\xF0\x9F\x98\x80z", "ASCII", "UTF-8", 1), "a_z");
	return check_done();
}
