/* Public domain
 * For use in mount command
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "natspec_internal.h"

static const char *list_fat_fs[] =
{
	"vfat", "fat", "msdos", NULL
};

static const char *list_io_fs[] =
{
	"cdfss", "iso9660", "udf", NULL
};

// FIXME: comma-separated list in str
static int str_in_list(const char *str, const char **list)
{
	for (;*list;list++)
	{
		DEBUG (printf("Comparing '%s' and '%s'\n", str, *list));
		if (!strcasecmp(str, *list))
			return 1;
	}
	return 0;
}

// FIXME: add bound control
static void add_option(char *options, const char *option1, const char *option2)
{
	if (options[0] && options[strlen(options)-1] != ',')
		strcat(options,",");
	strcat(options, option1);
	if (option2)
		strcat(options, option2);
	DEBUG(printf("add_option: '%s', result:%s\n",option1, options));
}

char* natspec_enrich_fs_options(const char *fs, const char *options)
{
	char buf[100], *ret; buf[0] = 0;
	const char *charset, *codepage;

	// Enriching options string only if it does not contains encoding options
	if (options && (strstr(options,"iocharset") || 
		strstr(options,"codepage") || strstr(options,"nls")))
		return strdup(options);

	// Getting typical values
	charset = natspec_get_filename_encoding("");
	codepage = natspec_get_charset_by_locale(NATSPEC_DOSCS, "");

	// Specially for each file system
	if ( str_in_list (fs, list_fat_fs) )
	{
		if (charset)
			add_option(buf, "iocharset=", charset);
		if (codepage)
			add_option(buf, "codepage=", codepage);
	} else
	if ( str_in_list (fs, list_io_fs) )
	{
		if (charset)
			add_option(buf, "iocharset=", charset);
	} else
	if ( !strcmp (fs, "ntfs"))
	{
		if (charset)
			add_option(buf, "nls=", charset);
	} else
	{
		DEBUG (fprintf(stderr, "NATSPEC: do not know %s fs\n",fs));
		return strdup(options);
	};
	ret = malloc ( (options ? strlen(options) : 0) + strlen(buf) + 1);
	strcpy (ret, (options ? options : "" ));
	add_option (ret, buf, NULL);
	return ret;
}
