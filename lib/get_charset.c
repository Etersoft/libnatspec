/*
    lib/get_charset.c

	charset and locale related functions

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
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "natspec_internal.h"
#include "data/get_charset_data.h"
#include "data/charset_names.h"


/* Internal: Helper for bsearch */
static int charset_locale_cmp( const void *name, const void *entry )
{
    const struct charsetrel_entry *rel = (const struct charsetrel_entry *)entry;
	DEBUG (printf ("Compare %s with %s\n", (const char*)name, rel->locale));
    return strcasecmp( (const char *)name, rel->locale );
}

/* Removes punctuation characters from charset name */
static char *clean_charset( const char *charset)
{
	int i, j;
	char *buf;
	if (!charset || !charset[0])
		return NULL;
	buf = malloc( strlen(charset) + 1 );
   	for (i = 0, j = 0; charset[i]; i++)
        if (isalnum(charset[i]))
			buf[j++] = charset[i];
   	buf[j] = 0;
	DEBUG (printf ("Clean charset '%s', after: '%s'",charset, buf));
	return buf;
}

/*
TODO: Приводит к виду, использующемуся в iconv
static char *normalize_charset(const char *charset)
{
	char *buf = malloc( strlen(charset) + 1 );
	strcpy(buf, charset);
	return buf;
}
*/

/* Returns charset get from _locale_ */
char *natspec_get_charset_from_locale(const char *locale)
{
	char *lang, *next, *dialect, *charset;
    if (!locale || !locale[0])
		return NULL;
	lang = malloc( strlen(locale) + 1 );
    strcpy( lang, locale );
    next = strchr(lang,':'); if (next) *next++='\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++='\0';
    charset = strchr(lang,'.'); if (charset) *charset++='\0';
	return clean_charset(charset);
}


static const char *get_cs_by_type(const int type,
	const struct charsetrel_entry* entry)
{
	if (!entry)
		return NULL;
	switch(type)
	{
		case NATSPEC_WINCS:
			return entry->win_cs;
		case NATSPEC_DOSCS:
			return entry->dos_cs;
		case NATSPEC_MACCS:
			return entry->mac_cs;
		case NATSPEC_UNIXCS:
			return entry->unix_cs;
		default:
			return NULL;
	}
}


/* Internal: try search by encoding */
static const struct charsetrel_entry* get_entry_by_charset(const int bytype,
	const char *charset, const char *locale)
{
	const struct charsetrel_entry *entry = NULL;
	char *must_free = NULL;
	DEBUG (printf("get_entry_by_charset charset=%s, locale=%s\n",charset, locale));
	/* Если не указана, но знаем локаль, получаем по системной локали */
	if (!charset && locale && bytype == NATSPEC_UNIXCS)
	{
		char *old;
		must_free = NULL;
		if (!locale[0])
			locale = must_free = natspec_get_system_locale();
		old = setlocale(LC_ALL,locale);
		if (must_free)
			free (must_free);
		charset = must_free = clean_charset(nl_langinfo(CODESET));
		DEBUG (printf("2 cs=%s\n",charset));
		setlocale(LC_ALL, old); /* is it correct return to previous settings? */
	}
	if (charset && charset[0])
	{
		int i;
		DEBUG (printf("Find with charset '%s'\n", charset));
		/* Fixme: can we suppose enemy encoding by unix charset? */
		for (i = 0; i< sizeof(charset_relation)/sizeof(charset_relation[0]); i++)
			if (!strcasecmp (charset, get_cs_by_type(bytype,&charset_relation[i])))
			{
	    		entry = &charset_relation[i];
				break;
			}
	}
	if (must_free)
		free(must_free);
	return entry;
}

/* Internal: Search _locale_ in list and returns entry pointer or NULL */
static const struct charsetrel_entry* get_entry_by_locale(const char *locale)
{
	char *charset;
	const struct charsetrel_entry *entry = NULL;
	char *buf = repack_locale(locale);
	/* Search the same locale string */
	if (buf && buf[0])
	{
		entry = bsearch( buf, charset_relation,
	    	sizeof(charset_relation)/sizeof(charset_relation[0]),
	        sizeof(charset_relation[0]), charset_locale_cmp );
		free (buf);
	}
	if (!entry)
	{
		/* It will broken brain if we have a few identical charset */
		charset = natspec_get_charset_from_locale(locale);
		DEBUG (printf ("Can't find the locale '%s', search by charset '%s' now\n", locale, charset));
		entry = get_entry_by_charset(NATSPEC_UNIXCS, charset, locale);
		free (charset);
	}
	return entry;
}

/***********************************************************************
 *              charset_cmp (internal)
 */

static int charset_cmp( const void *name, const void *entry )
{
    const struct charset_entry *charset = (const struct charset_entry *)entry;
    return strcasecmp( (const char *)name, charset->charset_name );
}

/* Returns codepage from charset */
static char __cfc[10]; /* FIXME */
const char *natspec_get_codepage_from_charset(const char *cs)
{
	const struct charset_entry *entry;
	entry = bsearch( cs, charset_names,
                     sizeof(charset_names)/sizeof(charset_names[0]),
                     sizeof(charset_names[0]), charset_cmp );
    if (entry)
	{
		sprintf(__cfc,"%d",entry->codepage);
		return __cfc;
	}
	else
		return cs;
}

/* Returns nls name (for kernel) from charset */
const char *natspec_get_nls_from_charset(const char *cs)
{
	const struct charset_entry *entry;
	entry = bsearch( cs, charset_names,
                     sizeof(charset_names)/sizeof(charset_names[0]),
                     sizeof(charset_names[0]), charset_cmp );
    if (entry)
		return entry->nls;
	else
		return "ascii"; /* ANSIX341968 */
}

const char * natspec_get_charset_by_locale(const int type, const char *locale)
{
	const struct charsetrel_entry* entry = get_entry_by_locale(locale);
	return get_cs_by_type(type, entry);
}

/*
const char * natspec_get_codepage_by_locale(const int type, const char *locale)
{
	const struct charset_entry* entry = get_entry_by_locale(locale);
	const char *cs = get_cs_by_type(type, entry);
	return natspec_codepage_from_charset(cs);
}
*/

const char * natspec_get_charset_by_charset(const int type,
	const int bytype, const char *charset)
{
	const struct charsetrel_entry* entry;
	entry = get_entry_by_charset(bytype, charset, "");
	return get_cs_by_type(type, entry);
}


const char *natspec_get_filename_encoding(const char *locale)
{
	const char *buf = getenv("G_FILENAME_ENCODING");
	if (buf)
	{
		/* Search in static table */
		buf = natspec_get_charset_by_charset(NATSPEC_UNIXCS, NATSPEC_UNIXCS, buf);
		if (buf)
			return buf;
	}
	return natspec_get_charset_by_locale(NATSPEC_UNIXCS, locale);
}

