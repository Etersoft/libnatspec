/*
    lib/get_locale.c

	locale related functions

    NATSPEC library
    This code contains some pieces of code and ideas from
                            WINE project (file dlls/kernel/locale.c)
                            GETTEXT project (file localcharset.c)
							GNOME project (file gnome-libs/libgnome/gnome-i18n.c)

    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: get_locale.c,v 1.10 2005/02/23 15:03:27 lav Exp $

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
#include <stdio.h>
#include <string.h>

#include "natspec_internal.h"

/* Try LANGUAGE:LC_ALL:LC_CTYPE:LANG from environment
   Returns NULL if locale is missed, empty or POSIX/C
*/
static char *get_from_env()
{
	char *tmp;
	/* The highest priority value is the `LANGUAGE' environment
	variable.  This is a GNU extension.  */
	tmp = getenv ("LANGUAGE");
	if (tmp == NULL || tmp[0] == '\0')
		tmp = getenv("LC_ALL");
	if (tmp == NULL || tmp[0] == '\0')
		tmp = getenv("LC_CTYPE");
	if (tmp == NULL || tmp[0] == '\0')
		tmp = getenv("LANG");
		
	if (tmp != NULL && tmp[0] != '\0' &&
		strcmp(tmp,"POSIX") && strcmp(tmp,"C") )
			return strdup(tmp);
	return NULL;
}

/* Returns user locale string (malloc allocated) */
char *natspec_get_user_locale()
{
	char *locale = get_from_env();
	if (!locale)
		return natspec_get_system_locale();
	return locale;
}

/*
 * TODO: fix ugly buf, fgets
*/
/* Read system wide locale, return str or NULL if it does not exist */
static char *get_from_system_i18n(const char *str)
{
	int i;
	char *locale = NULL;
	FILE *fd;
	fd = fopen("/etc/sysconfig/i18n","r");
	for (;fd;)
	{
		char buf[100], buf1[100];
		char *r;
		/* Read next line */
		r = fgets(buf,99,fd);
		if (!r) break;
		/* Remove space symbols FIXME: some glibc func?*/
		for (i = 0; *r; r++)
		{
			switch (*r)
			{
				case ' ':
				case '\n':
				case '\t':
					break;
				default:
					buf1[i++] = *r;
			}
		}
		buf1[i] = 0;
		DEBUG (printf("GSL: after space removing '%s'",buf1));
		i = strlen(str);
		if (!strncmp(buf1, str, i) && buf1[i] == '=')
		{
			locale = strdup ( buf1+i+1 );
			break;
		}
	}
	if (fd)
		fclose(fd);
	return locale;
}

/* Returns system locale string (malloc allocated) */
char *natspec_get_system_locale()
{
	char *locale = get_from_system_i18n("LANG");
	if (locale == NULL)
		locale = get_from_env();
	/* FIXME: we get SegFault in library (when empty sysconfig/i18n and POSIX locale) if return NULL */
	if (locale == NULL)
		locale = strdup("POSIX");
	return locale;
}


/* Returns charset get from _locale_ */
char *natspec_extract_charset_from_locale(const char *locale)
{
	char *lang, *next, *dialect, *charset, *ret;
    if (!locale || !locale[0])
		return NULL;
	lang = strdup( locale );
    next = strchr(lang,':'); if (next) *next++='\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++='\0';
    charset = strchr(lang,'.'); if (charset) *charset++='\0';
	ret = natspec_humble_charset(charset);
	free (lang);
	return ret;
}


/* Internal: repack locale string (compress charset) */
char *repack_locale(const char *locale)
{
	char *buf, *lang, *next, *dialect, *charset, *country;
    if (!locale || !locale[0] ||
		!strcmp(locale,"POSIX") || !strcmp(locale,"C") )
		return NULL;
	DEBUG (printf("repack_locale\n"));
	lang = strdup( locale );
	buf = malloc( strlen(locale) + 1 );
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
	charset = natspec_extract_charset_from_locale(locale);
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
