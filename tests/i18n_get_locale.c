/*
 * Public domain
 *
 * lib/get_locale.c with the system i18n file name taken from
 * NATSPEC_TEST_I18N_FILE environment variable (for test_env only)
 */

#include <stdlib.h>
#include "natspec_internal.h"

static const char *natspec_test_i18n_file(void)
{
	const char *file = getenv("NATSPEC_TEST_I18N_FILE");
	return file ? file : "/nonexistent/i18n";
}

#undef NATSPEC_I18N_FILE
#define NATSPEC_I18N_FILE natspec_test_i18n_file()

#include "get_locale.c"
