/*
 * Public domain
 *
 * libFuzzer harness: parsing of locale and charset names,
 * lookups in the tables and mount options.
 * Input: string; the part after the first '\n' is mount options.
 */

#include <stdlib.h>
#include <string.h>

#include "natspec.h"

char *_natspec_repack_locale(const char *locale);
int natspec_isutf8(const char *buf);

static const char *fs_list[] = {
	"vfat", "fat", "msdos", "iso9660", "udf", "jfs", "ntfs", "smbfs", "cifs", "ext4", NULL
};

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
	char *str, *options, *p;
	int t, i;

	str = malloc(size + 1);
	memcpy(str, data, size);
	str[size] = '\0';
	options = strchr(str, '\n');
	if (options)
		*options++ = '\0';

	free(natspec_humble_charset(str));
	free(natspec_extract_charset_from_locale(str));
	free(_natspec_repack_locale(str));
	for (t = NATSPEC_UNIXCS; t <= NATSPEC_MACCS; t++)
	{
		const char *cs = natspec_get_charset_by_locale(t, str);
		if (!cs)
			abort(); /* never NULL */
		natspec_get_charset_by_charset(t, NATSPEC_UNIXCS, str);
		natspec_get_charset_by_charset(t, NATSPEC_WINCS, str);
	}
	natspec_locale_is_utf8(str);
	if (!natspec_get_filename_encoding(str))
		abort();
	natspec_get_codepage_by_charset(str);
	if (!natspec_get_nls_by_charset(str))
		abort();
	natspec_isutf8(str);

	/* the current locale and G_FILENAME_ENCODING from the input */
	setenv("LC_ALL", str, 1);
	setenv("G_FILENAME_ENCODING", str, 1);
	free(natspec_get_current_locale());
	natspec_get_filename_encoding("");
	for (i = 0; fs_list[i]; i++)
		free(natspec_get_enriched_fs_options(fs_list[i], options));
	free(natspec_get_enriched_fs_options(str, options));
	p = natspec_get_enriched_fs_options(options ? options : str, str);
	free(p);
	unsetenv("LC_ALL");
	unsetenv("G_FILENAME_ENCODING");

	free(str);
	return 0;
}
