/*
 * Public domain
 *
 * Known bugs: built-in tables break bsearch
 * - two POSIX entries are at the end of charset_relation, after zu_ZA.UTF8
 * - C, C.UTF8, POSIX keys can never match (locale is lowercased by repack)
 * - unicode_7b has duplicate keys (0x00ad, 0x0280, 0x0334, 0x2282)
 */

#include "check.h"
#include "natspec_internal.h"
#include "data/get_charset_data.h"
#include "unicode/uni_7b.h"

#define COUNT(a) (sizeof(a) / sizeof(a[0]))

char *_natspec_repack_locale(const char *locale);

static void test_charset_relation(void)
{
	size_t i;
	for (i = 0; i < COUNT(charset_relation); i++)
	{
		const char *locale = charset_relation[i].locale;
		char *repacked = _natspec_repack_locale(locale);
		check_total++;
		if (!repacked || strcmp(repacked, locale))
		{
			check_failed++;
			fprintf(stderr, "FAIL: charset_relation: '%s' can not be found (repacked to '%s')\n",
				locale, repacked ? repacked : "(null)");
		}
		free(repacked);
		check_total++;
		if (i && strcmp(charset_relation[i-1].locale, locale) >= 0)
		{
			check_failed++;
			fprintf(stderr, "FAIL: charset_relation: '%s' is not sorted after '%s'\n",
				locale, charset_relation[i-1].locale);
		}
	}
}

static void test_unicode_7b(void)
{
	size_t i;
	for (i = 1; i < COUNT(unicode_7b); i++)
	{
		check_total++;
		if (unicode_7b[i-1].x >= unicode_7b[i].x)
		{
			check_failed++;
			fprintf(stderr, "FAIL: unicode_7b: duplicate or unsorted 0x%04x\n", unicode_7b[i].x);
		}
	}
}

int main(void)
{
	test_charset_relation();
	test_unicode_7b();
	return check_done();
}
