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

    $Id: get_locale.c,v 1.22 2008/02/14 22:37:08 vitlav Exp $

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA.
*/

#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "natspec_internal.h"

/*! Try LC_ALL:LC_CTYPE:LANG from environment
   Returns NULL if locale is missed, empty or POSIX/C
   \todo FIXME: is it equivalent of setlocale(LC_ALL,"")?
   \todo FIXME: LANGUAGE can has multiple value (and ':')
*/
/* From gettext(3):
> If the LANGUAGE environment variable is set to a  nonempty  value,  and
> the  locale  is not the "C" locale, the value of LANGUAGE is assumed to
> contain a colon separated list of  locale  names.  The  functions  will
> attempt  to  look  up  a translation of msgid in each of the locales in
> turn.  This is a GNU extension.
*/

char *natspec_internal_get_locale_from_env()
{
	char *tmp, *next;
	/* The highest priority value is the `LANGUAGE' environment
	variable.  This is a GNU extension.  */
/*	tmp = getenv ("LANGUAGE");
	if (_n_isempty(tmp)) */
	tmp = getenv("LC_ALL");
	if (_n_isempty(tmp))
		tmp = getenv("LC_CTYPE");
	if (_n_isempty(tmp))
		tmp = getenv("LANG");
	if (_n_isempty(tmp) ||
		!strcmp(tmp, "POSIX") || !strcmp(tmp, "C") )
			return NULL;
	tmp = strdup(tmp);
	if (!tmp)
		return NULL;
	/* Use only first locale int string */
	next = strchr(tmp,':'); if (next) *next++ = '\0';
	return tmp;
}

/*! Returns user locale string (malloc allocated)
 * \todo FIXME: can we do it with glibc not my get_from_env?
 */
char *natspec_get_current_locale()
{
	char *locale = natspec_internal_get_locale_from_env();
	if (locale == NULL)
		return natspec_get_system_locale();
	return locale;
}

char *natspec_get_user_locale()
{
	return natspec_get_current_locale();
}

/*
const char *get_i18n_filename()
{
	return "/etc/sysconfig/i18n";
}
*/

/* File with locale information:
 *   SuSE         - /etc/rc.local       RC_LANG=
 *   RedHat-based - /etc/sysconfig/i18n LANG=
 *   Debian       - /etc/enviroment     LANG=
 *   Slackware    - /etc/profile        export LANG=
 *   FreeBSD      - /etc/profile        LANG=ru_RU.KOI8-R; export LANG
*/

/*! Read system wide locale, return str or NULL if it does not exist
 *  Since POSIX:1996 locale info we get from LANG variable
 *  Supported syntax of the line (shell-like):
 *    [export] NAME = [quote]value[quote] [; or # comment]
*/
static char *get_from_system_i18n(const char *str)
{
	char *locale = NULL;
	char *line = NULL;
	size_t size = 0;
	size_t len = strlen(str);
	FILE *fd;
	fd = fopen(NATSPEC_I18N_FILE,"r");
	if (!fd)
		return NULL;
	while (!locale && getline(&line, &size, fd) != -1)
	{
		char *p = line, *value;
		char quote = 0;
		p += strspn(p, " \t");
		if (!strncmp(p, "export", 6) && (p[6] == ' ' || p[6] == '\t'))
		{
			p += 6;
			p += strspn(p, " \t");
		}
		/* NAME must be at the beginning of the line */
		if (strncmp(p, str, len))
			continue;
		p += len;
		p += strspn(p, " \t");
		if (*p != '=')
			continue;
		p++;
		p += strspn(p, " \t");
		if (*p == '"' || *p == '\'')
			quote = *p++;
		value = p;
		if (quote)
			p = strchr(p, quote);
		else
			p += strcspn(p, " \t;#\r\n");
		if (!p)
			continue; /* unterminated quote */
		*p = '\0';
		DEBUG (printf("GSL: %s='%s'\n", str, value));
		if (*value)
			locale = strdup(value);
	}
	free(line);
	fclose(fd);
	return locale;
}

/*! Returns system locale string (malloc allocated) */
char *natspec_get_system_locale()
{
	char *locale = get_from_system_i18n("LANG");
	if (locale == NULL)
		locale = natspec_internal_get_locale_from_env();
	/*! \todo FIXME: we get SegFault in library (when empty sysconfig/i18n and POSIX locale) if return NULL */
	if (locale == NULL)
		locale = strdup("C");
	return locale;
}


/*! Returns charset get from _locale_ */
char *natspec_extract_charset_from_locale(const char *locale)
{
	char *lang, *next, *dialect, *charset, *ret;
	if (_n_isempty(locale))
		return NULL;
	lang = strdup( locale );
	if (!lang)
		return NULL;
	next = strchr(lang,':'); if (next) *next++ = '\0';
	dialect = strchr(lang,'@'); if (dialect) *dialect++ = '\0';
	charset = strchr(lang,'.'); if (charset) *charset++ = '\0';
	ret = natspec_humble_charset(charset);
	free (lang);
	return ret;
}


/*! Internal: repack locale string (compress charset, fix register) */
char *_natspec_repack_locale(const char *locale)
{
	int i;
	char *buf, *lang, *next, *dialect, *charset, *country;
	if (_n_isempty(locale))
		return NULL;
	DEBUG (printf("repack_locale\n"));
	lang = strdup( locale );
	/* normalized charset can be longer than in locale (cp866 -> IBM866) */
	charset = natspec_extract_charset_from_locale(locale);
	buf = malloc( strlen(locale) + (charset ? strlen(charset) : 0) + 2 );
	if (!lang || !buf)
	{
		free(lang);
		free(buf);
		free(charset);
		return NULL;
	}
	next = strchr(lang,':'); if (next) *next++ = '\0';
	dialect = strchr(lang,'@'); if (dialect) *dialect++ = '\0';
	next = strchr(lang,'.'); if (next) *next = '\0';
	country = strchr(lang,'_'); if (country) *country++ = '\0';

	/* C and POSIX locale names are case sensitive */
	if (strcmp(lang, "C") && strcmp(lang, "POSIX"))
		for (i=0; i<strlen(lang); i++)
			lang[i] = _n_tolower(lang[i]);
	strcpy(buf, lang);
	if (country)
	{
		for (i=0; i<strlen(country); i++)
			country[i] = _n_toupper(country[i]);
		strcat(buf, "_");
		strcat(buf, country);
	}
	if (charset)
	{
		strcat(buf, ".");
		strcat(buf, charset);
		free (charset);
	}
	if (dialect)
	{
		for(i=0; i<strlen(dialect); i++)
			dialect[i] = _n_tolower(dialect[i]);
		strcat(buf, "@");
		strcat(buf, dialect);
	}
	free (lang);
	return buf;
}
