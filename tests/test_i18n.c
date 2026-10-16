/*
 * Public domain
 *
 * Syntax of the system i18n file (get_from_system_i18n):
 * LANG must be at the beginning of the line (SUPPORTED_LANG is not LANG),
 * long lines, shell syntax (export, quotes)
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

/* NATSPEC_I18N_FILE can be ':' separated list, the first file with LANG wins */
static void check_list(const char *content1, const char *content2, const char *expected)
{
	char *f1 = test_write_file(content1), *f2 = test_write_file(content2);
	char *list = malloc(strlen(f1) + strlen(f2) + 32), *got;
	sprintf(list, "/nonexistent/locale.conf:%s:%s", f1, f2);
	setenv("NATSPEC_TEST_I18N_FILE", list, 1);
	got = natspec_get_system_locale();
	check_total++;
	if (got == NULL || strcmp(got, expected))
	{
		check_failed++;
		fprintf(stderr, "FAIL: system locale for list '%s' + '%s': got '%s', expected '%s'\n",
			content1, content2, got ? got : "(null)", expected);
	}
	free(got);
	free(list);
	test_remove_file(f1);
	test_remove_file(f2);
	unsetenv("NATSPEC_TEST_I18N_FILE");
}

static void test_file_list(void)
{
	check_list("LANG=ru_RU.UTF-8\n", "LANG=uk_UA.UTF-8\n", "ru_RU.UTF-8");
	check_list("LC_TIME=C\n", "LANG=uk_UA.UTF-8\n", "uk_UA.UTF-8");
	check_list("", "", "C");
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
	check_i18n("\texport  LANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	check_i18n("LANG='uk_UA.UTF-8'\n", "uk_UA.UTF-8");
	check_i18n("LANG=\"uk_UA.UTF-8\" # comment\n", "uk_UA.UTF-8");
	check_i18n("LANGUAGE=de_DE\nLANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	/* broken and empty values are skipped */
	check_i18n("LANG=\"uk_UA.UTF-8\nLANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	check_i18n("LANG=\nLANG=ru_RU.UTF-8\n", "ru_RU.UTF-8");
	check_i18n("LANG=\n", "C");

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

	test_file_list();
	return check_done();
}
