/*
 * Public domain
 *
 * Known bug: empty charset crashes public lookup functions
 * (natspec_humble_charset returns NULL which goes to bsearch)
 */

#include "check.h"
#include "natspec.h"

int main(void)
{
	CHECK_INT(natspec_get_codepage_by_charset(""), 437);
	CHECK_INT(natspec_get_codepage_by_charset(NULL), 437);
	CHECK_STR(natspec_get_nls_by_charset(""), "iso8859-1");
	CHECK_STR(natspec_get_nls_by_charset(":"), "iso8859-1");
	return check_done();
}
