/*
 * Public domain
 *
 * libFuzzer harness: parsing of the system i18n file.
 * Linked with ../i18n_get_locale.c, the file is NATSPEC_TEST_I18N_FILE.
 * Input: content of the file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "natspec.h"

static char file[64];

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
	FILE *f;
	char *locale;

	if (!*file)
	{
		int fd;
		strcpy(file, "/tmp/natspec-fuzz-i18n-XXXXXX");
		fd = mkstemp(file);
		if (fd < 0)
			abort();
		close(fd);
		unsetenv("LC_ALL");
		unsetenv("LC_CTYPE");
		unsetenv("LANG");
		setenv("NATSPEC_TEST_I18N_FILE", file, 1);
	}
	f = fopen(file, "w");
	if (!f)
		abort();
	fwrite(data, 1, size, f);
	fclose(f);

	locale = natspec_get_system_locale();
	if (!locale)
		abort(); /* never NULL */
	free(locale);
	return 0;
}
