/*
 * Public domain
 *
 * Known bugs in transliteration data and logic:
 * - character outside BMP gives one '_' per byte
 */

#include "check.h"
#include "natspec.h"

int main(void)
{
	/* U+1F600 GRINNING FACE */
	CHECK_STR_FREE(natspec_convert("a\xF0\x9F\x98\x80z", "ASCII", "UTF-8", 1), "a_z");
	return check_done();
}
