/*
    lib/get_locale.c

	locale related functions

    NATSPEC library
    This code contains some pieces of code and ideas from
                            WINE project (file dlls/kernel/locale.c)
                            GETTEXT project (file localcharset.c)
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

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

#include <stdlib.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>

/*#include "data/charset_names.h"*/
#include "natspec_internal.h"


/* Returns system locale string (malloc allocated)
NB TODO: fix ugly malloc, fgets, and from LANG copying using
TODO: allowing space symbols in file
TODO: Как-то должна учитываться ~/.i18n ?
*/
char *natspec_get_system_locale()
{
	char *locale = malloc(100);
	char *tmp;

	// Try LANG from environment
	tmp = getenv("LANG");
	if (tmp && tmp[0]) // Use only if non empty string
	{
		strcpy(locale, tmp);
		return locale;
	}

	// Read system wide locale
	FILE *fd = fopen("/etc/sysconfig/i18n","r");
	for (;fd;)
	{
		char *r;
		char buf[100];
		r = fgets(buf,99,fd);
		DEBUG (printf("GSL: get string '%s'",buf));
		if (!r) break;
		/* FIXME: space symbols!! */
		r = strstr(buf,"LANG=");
		if (r)
		{
			int i;
			r += 5;
			for (i=0;*r;r++)
			{
				switch (*r)
				{
					case ' ':
					case '\n':
					case '\t':
					case '=':
						break;
					default:
						locale[i++] = *r;
				}
			}
			locale[i] = 0;
			break;
		}
	}
	if (fd)
		fclose(fd);
	return locale;
}

/* Internal: repack locale string (compress charset) */
char *repack_locale(const char *locale)
{
	char *buf, *lang, *next, *dialect, *charset, *country;
    if (!locale || !locale[0] ||
		!strcmp(locale,"POSIX") || !strcmp(locale,"C") )
		return NULL;
	// Cut : and @
	DEBUG (printf("repack_locale\n"));
	lang = malloc( strlen(locale) + 1 );
	buf = malloc( strlen(locale) + 1 );
    strcpy( lang, locale );
    next = strchr(lang,':'); if (next) *next++='\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++='\0';
    charset = strchr(lang,'.'); if (charset) *charset++='\0';
    country = strchr(lang,'_'); if (country) *country++='\0';
	strcpy(buf, lang);
	if (country)
	{
		strcat(buf, "_");
		strcat(buf, country);
	}
	charset = natspec_get_charset_from_locale(locale);
	if (charset)
	{
		strcat(buf, ".");
		strcat(buf, charset);
		free (charset);
	}
	if (dialect)
	{
		strcat(buf, "@");
		strcat(buf, dialect);
	}
	free (lang);
	return buf;
}
