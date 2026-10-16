/*
 * Public domain
 *
 * Consistency checks for built-in tables (they are searched with bsearch)
 * Strict checks which fail now are in xfail_tables.c
 */

#include "check.h"
#include "natspec_internal.h"
#include "data/charset_names.h"
#include "unicode/uni_7b.h"

#define COUNT(a) (sizeof(a) / sizeof(a[0]))

static void test_charset_names(void)
{
	size_t i;
	for (i = 0; i < COUNT(charset_names); i++)
	{
		const char *name = charset_names[i].charset_name;
		const char *c;
		/* names must be in natspec_humble_charset form */
		for (c = name; *c; c++)
			if (!((*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9')))
				break;
		check_total++;
		if (*c || !*name)
		{
			check_failed++;
			fprintf(stderr, "FAIL: charset_names: '%s' is not normalized\n", name);
		}
		check_total++;
		if (i && strcmp(charset_names[i-1].charset_name, name) >= 0)
		{
			check_failed++;
			fprintf(stderr, "FAIL: charset_names: '%s' is not sorted after '%s'\n",
				name, charset_names[i-1].charset_name);
		}
		CHECK(charset_names[i].nls && *charset_names[i].nls);
		CHECK(charset_names[i].codepage > 0);
	}
}

static void test_unicode_7b(void)
{
	size_t i;
	for (i = 1; i < COUNT(unicode_7b); i++)
	{
		check_total++;
		if (unicode_7b[i-1].x > unicode_7b[i].x)
		{
			check_failed++;
			fprintf(stderr, "FAIL: unicode_7b: 0x%04x is not sorted\n", unicode_7b[i].x);
		}
	}
	/* table must be full for bsearch over sizeof */
	CHECK(unicode_7b[COUNT(unicode_7b) - 1].x != 0);
}

int main(void)
{
	test_charset_names();
	test_unicode_7b();
	return check_done();
}
