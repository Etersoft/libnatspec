/* This code contains some pieces of code and ideas from
 *                                WINE project (file dlls/kernel/locale.c)
 *                                GETTEXT project (file localcharset.c)
 * I hope it pieces too small and I can release this code in public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

#include <stdlib.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>

#include "data/get_charset_data.h"
#include "natspec_internal.h"

/* Internal: Helper for bsearch */
static int charset_locale_cmp( const void *name, const void *entry )
{
    const struct charset_entry *rel = (const struct charset_entry *)entry;
	DEBUG (printf ("Compare %s with %s\n", name, rel->locale));
    return strcasecmp( (const char *)name, rel->locale );
}

/* remove punctuation characters from charset name */
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

// TODO: Приводит к виду, использующемуся в iconv
static char *normalize_charset(const char *charset)
{
	char *buf = malloc( strlen(charset) + 1 );
	strcpy(buf, charset);
	return buf;
}

/* Returns charset get from _locale_ */
char *natspec_get_charset_from_locale(const char *locale)
{
	char *buf, *lang, *next, *dialect, *charset, *country;
    if (!locale || !locale[0])
		return NULL;
	lang = malloc( strlen(locale) + 1 );
    strcpy( lang, locale );
    next = strchr(lang,':'); if (next) *next++='\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++='\0';
    charset = strchr(lang,'.'); if (charset) *charset++='\0';
	return clean_charset(charset);
}

/* Returns system locale string (malloc allocated)
NB TODO: fix ugly malloc, fgets, and from LANG copying using
*/
char *natspec_get_system_locale()
{
	char *locale = malloc(100);
	locale[0] = '\0';
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
	// If can't read sysconfig, try LANG from environment
	if (!locale[0])
		strcpy(locale, getenv("LANG"));
	return locale;
}

static char *repack_locale(const char *locale)
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

static const char *get_cs_by_type(const int type,
	const struct charset_entry* entry)
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


// try search by encoding
static const struct charset_entry* get_entry_by_charset(const int bytype,
	const char *charset, const char *locale)
{
	const struct charset_entry *entry = NULL;
	char *must_free = NULL;
	DEBUG (printf("get_entry_by_charset charset=%s, locale=%s\n",charset, locale));
	// Если не указана, но знаем локаль, получаем из системы
	// (только если получить надо UNIXCS)
	if (!charset && locale && bytype == NATSPEC_UNIXCS)
	{
		must_free = NULL;
		if (!locale[0])
			locale = must_free = natspec_get_system_locale();
		char *old = setlocale(LC_ALL,locale);
		if (must_free)
			free (must_free);
		charset = must_free = clean_charset(nl_langinfo(CODESET));
		DEBUG (printf("2 cs=%s\n",charset));
		setlocale(LC_ALL, old); // is it correct return to previous settings?
	}
	if (charset && charset[0])
	{
		DEBUG (printf("Find with charset '%s'\n", charset));
		int i;
		// Fixme: can we suppose enemy encoding by unix charset?
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
static const struct charset_entry* get_entry_by_locale(const char *locale)
{
	char *charset;
	const struct charset_entry *entry = NULL;
	char *buf = repack_locale(locale);
	// Search the same locale string
	if (buf && buf[0])
	{
		entry = bsearch( buf, charset_relation,
	    	sizeof(charset_relation)/sizeof(charset_relation[0]),
	        sizeof(charset_relation[0]), charset_locale_cmp );
		free (buf);
	}
	if (!entry)
	{
		charset = natspec_get_charset_from_locale(locale);
		DEBUG (printf ("Can't find the locale '%s', search by charset '%s' now\n", locale, charset));
		entry = get_entry_by_charset(NATSPEC_UNIXCS, charset, locale);
		free (charset);
	}
	return entry;
}

const char * natspec_get_charset_by_locale(const int type, const char *locale)
{
	const struct charset_entry* entry = get_entry_by_locale(locale);
	return get_cs_by_type(type, entry);
}

const char * natspec_get_charset_by_charset(const int type,
	const int bytype, const char *charset)
{
	const struct charset_entry* entry;
	entry = get_entry_by_charset(bytype, charset, "");
	return get_cs_by_type(type, entry);
}

const char *natspec_get_filename_encoding(const char *locale)
{
	char *buf = getenv("G_FILENAME_ENCODING");
	if (buf)
		return buf;
	return natspec_get_charset_by_locale(NATSPEC_UNIXCS, locale);
}

