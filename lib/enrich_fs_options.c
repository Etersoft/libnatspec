/*
    lib/enrich_fs_options.c
	
	support for add locale related options to mount

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: enrich_fs_options.c,v 1.19 2005/03/02 18:22:07 lav Exp $

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation, version 2.1
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
    USA.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "natspec_internal.h"

/* FAT/CDFS: iocharset & codepage */
static const char *list_fat_fs[] =
{
	"vfat", "fat", "msdos", NULL
};

/* SMB/CIFS: iocharset & codepage */
static const char *list_smb_fs[] =
{
	"smb", "smbfs", "cifs", NULL
};

/* iocharset (jfs use Unicode internal, add it too */
static const char *list_io_fs[] =
{
	"iso9660", "udf", "jfs", NULL
};

/* If see this type of fs, skip enriching */
static const char *list_enc_opts[] =
{
	"iocharset", "codepage", "nls", NULL
};

/* FIXME: test for comma-separated list in str */
/* Is there str in list */
static int str_in_list(const char *str, const char **list)
{
	for (;*list != NULL;list++)
	{
		DEBUG (printf("Comparing '%s' and '%s'\n", str, *list));
		if (!strcasecmp(str, *list))
			return 1;
	}
	return 0;
}

/* Is there one string from list in str? */
static int strstr_in_list(const char *str, const char **list)
{
	if (str == NULL)
		return 0;
	for (;*list != NULL;list++)
	{
		DEBUG (printf("Comparing '%s' and '%s'\n", str, *list));
		if (strstr(str, *list))
			return 1;
	}
	return 0;
}

/* FIXME: add bound control
 * Internal: adds new option1,2 to options param
 */
static void add_option(char *options, const char *option1, const char *option2)
{
	if (strlen(option1) == 0)
		return;
	if (options[0] != '\0' && options[strlen(options)-1] != ',')
		strcat(options,",");
	strcat(options, option1);
	if (option2)
		strcat(options, option2);
	DEBUG(printf("add_option: '%s', result:%s\n",option1, options));
}

/* Internal: adds options for filesystem fs */
static void add_options(char *buf, const char *fs)
{
	const char *charset, *codepage;
	if (fs == NULL)
		return;
	/* charset of our system */
	charset = natspec_get_filename_encoding("");
	/* codepage DOS system as assumes by current locale */
	codepage = natspec_get_codepage_by_charset(
		natspec_get_charset_by_locale(NATSPEC_DOSCS, ""));
	DEBUG (fprintf (stderr,"fn=%s ENRICH: codepage=%s\n",charset,codepage));

	/* Specially for each file system */
	if ( str_in_list (fs, list_fat_fs) )
	{
		if (charset)
			add_option(buf, "iocharset=", charset);
		if (codepage)
			add_option(buf, "codepage=", codepage);
	}
	
	else if ( str_in_list (fs, list_io_fs) )
	{
		if (charset)
			add_option(buf, "iocharset=", charset);
	}
	
	else if ( !strcmp (fs, "ntfs"))
	{
		if (charset)
			add_option(buf, "nls=", charset);
	}
	
	else if ( str_in_list (fs, list_smb_fs) )
	{
		/* smb has some specifity with codepage names */
		codepage = natspec_get_nls_by_charset(
			natspec_get_charset_by_locale(NATSPEC_DOSCS, ""));
		if (charset)
			add_option(buf, "iocharset=", charset);
		if (codepage)
			add_option(buf, "codepage=", codepage);
	}
	
	else
		DEBUG (fprintf(stderr, "NATSPEC: do not know %s fs\n",fs));
}

/* Add needed i18n options for fs type */
char* natspec_get_enriched_fs_options(const char *fs, const char *options)
{
	char *ret;
	char buf[100]; buf[0] = '\0';

	/* if encoding issues already exist in options, do nothing */
	if ( strstr_in_list (options, list_enc_opts) )
		return strdup(options);

	add_options(buf, fs);
	if (options == NULL)
		return strdup (buf);

	ret = malloc ( strlen(options) + strlen(buf) + 1 + 1 );
	strcpy (ret, options);
	add_option (ret, buf, NULL);
	return ret;
}

/* Obsoletes */
char* natspec_enrich_fs_options(const char *fs, char **options)
{
	char *opt;
	opt = natspec_get_enriched_fs_options(fs,*options);
	free(*options);
	*options = opt;
	return opt;
}
