/*
 * Public domain
 *
 * Known bugs: built-in tables break bsearch
 * - two POSIX entries are at the end of charset_relation, after zu_ZA.UTF8
 * - C, C.UTF8, POSIX keys can never match (locale is lowercased by repack)
 */

#include "check.h"
#include "natspec_internal.h"
#include "data/get_charset_data.h"

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

int main(void)
{
	test_charset_relation();
	return check_done();
}
