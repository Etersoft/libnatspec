/*
 * Public domain
 *
 * Empty or NULL charset in lookup functions gives the default value
 * (natspec_humble_charset returns NULL for it)
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
