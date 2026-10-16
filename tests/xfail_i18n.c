/*
 * Public domain
 *
 * Known bugs in /etc/sysconfig/i18n parsing (get_from_system_i18n):
 * - LANG is searched in any position of line
 * - long lines are read by 99 bytes chunks
 * - shell syntax 'export LANG=' is not supported
 *
 * Linked with i18n_get_locale.c, the system i18n file is taken
 * from NATSPEC_TEST_I18N_FILE
 */

#include "check.h"
#include "natspec.h"
#include "test_util.h"

static void check_i18n(const char *content, const char *expected)
{
	char *file = test_write_file(content);
	char *got;
	setenv("NATSPEC_TEST_I18N_FILE", file, 1);
	got = natspec_get_system_locale();
	check_total++;
	if (got == NULL || strcmp(got, expected))
	{
		check_failed++;
		fprintf(stderr, "FAIL: system locale for i18n file '%.40s...': got '%s', expected '%s'\n",
			content, got ? got : "(null)", expected);
	}
	free(got);
	test_remove_file(file);
}

int main(void)
{
	char buf[300];
	unsetenv("LC_ALL");
	unsetenv("LC_CTYPE");
	unsetenv("LANG");

	check_i18n("SUPPORTED_LANG=de_DE\nLANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	check_i18n("RC_LANG=de_DE\nLANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	check_i18n("export LANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");

	/* the tail of long comment must not be parsed */
	memset(buf, 0, sizeof(buf));
	buf[0] = '#';
	memset(buf + 1, 'x', 97);
	strcat(buf, " LANG=evil\nLANG=ru_RU.UTF-8\n");
	check_i18n(buf, "ru_RU.UTF-8");

	/* long line with LANG at the end */
	memset(buf, 0, sizeof(buf));
	strcpy(buf, "SYSFONT=");
	memset(buf + strlen(buf), 'x', 120);
	strcat(buf, "\nLANG=ru_RU.UTF-8\n");
	check_i18n(buf, "ru_RU.UTF-8");

	return check_done();
}
