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
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#include "natspec_internal.h"

#include "data/get_charset_data.h"

#ifdef HAVE_LANGINFO_H
# include <langinfo.h>
#endif

/* Returns current charset */
const char *natspec_get_charset()
{
#ifdef HAVE_GLIB
	const gchar *charset;
	g_get_charset(&charset);
	return charset;
#elif defined(HAVE_LANGINFO_CODESET)
	return nl_langinfo(CODESET);
#else
	#error "I have not an idea how do it"
	/* recursive :) FIXME: I
	return natspec_get_charset_by_locale(NATSPEC_UNIXCS, "");*/
#endif
}


/* Internal: Helper for bsearch */
static int charset_locale_cmp( const void *name, const void *entry )
{
    const struct charsetrel_entry *rel = (const struct charsetrel_entry *)entry;
	DEBUG (printf ("Compare %s with %s\n", (const char*)name, rel->locale));
    return strcasecmp( (const char *)name, rel->locale );
}

/* Removes punctuation characters from charset name */
char *natspec_humble_charset( const char *charset)
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

static const char *get_cs_by_type(const int type,
	const struct charsetrel_entry* entry)
{
	if (!entry)
		return NULL;
	switch(type)
	{
		case NATSPEC_WINCS:
			return ( entry->win_cs ? entry->win_cs : "CP1252");
		case NATSPEC_DOSCS:
			return ( entry->dos_cs ? entry->dos_cs : "CP437");
		case NATSPEC_MACCS:
			return ( entry->mac_cs ? entry->mac_cs : "MAC");
		case NATSPEC_UNIXCS:
			return ( entry->unix_cs ? entry->unix_cs : "iso8859-1"); /* Really? */
		default:
			break;
	}
	return "ascii";
}


/* Internal: try search in charset_relation by encoding (troubles with the same enc*/
static const struct charsetrel_entry* get_entry_by_charset(const int bytype,
	const char *charset, const char *locale)
{
	const struct charsetrel_entry *entry = NULL;
	char *must_free = NULL;
	DEBUG (printf("get_entry_by_charset charset=%s, locale=%s\n",charset, locale));
	/* Если не указана charset, но знаем локаль, получаем по локали */
	if (!charset && locale && bytype == NATSPEC_UNIXCS)
	{
		char *old;
		/* If locale does not specify, use system locale */
		if (!locale[0])
			locale = must_free = natspec_get_system_locale();
		old = setlocale(LC_ALL, locale);
		if (must_free)
			free (must_free);
		/* Get charset for this locale */
		charset = must_free = natspec_humble_charset(natspec_get_charset());
		DEBUG (printf("2 cs=%s\n", charset));
		setlocale(LC_ALL, old); /*is it correct return to previous settings? */
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
	if (must_free != NULL)
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
	/* If can't find by locale, try by charset identity */
	if (!entry)
	{
		/* FIXME: It will broken brain if we have a few identical charset */
		charset = natspec_extract_charset_from_locale(locale);
		DEBUG (printf ("Can't find the locale '%s', search by charset '%s' now\n", locale, charset));
		entry = get_entry_by_charset(NATSPEC_UNIXCS, charset, locale);
		free (charset);
	}
	return entry;
}


/* Returns charset for locale */
const char * natspec_get_charset_by_locale(const int type, const char *locale)
{
	const struct charsetrel_entry* entry;
	if (!locale)
		locale = natspec_get_system_locale();
	entry = get_entry_by_locale(locale);
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
