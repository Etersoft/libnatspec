/*
    lib/natspec.h
	
	main header file

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: natspec.h,v 1.12 2005/02/23 13:39:23 lav Exp $

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

#ifndef __NATSPEC_H
#define __NATSPEC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Types of character set tables for various OS */

#define NATSPEC_UNIXCS 0
#define NATSPEC_WINCS 1
#define NATSPEC_DOSCS 2
#define NATSPEC_MACCS 3

/* 
 * Returns malloc allocated string with system locale
 * get from LC_ALL:LC_CTYPE:LANG evironment variable, if fails,
 * get from get_system_locale()
 */
 
char *natspec_get_user_locale();

/* 
 * Returns malloc allocated string with user locale
 * get from LANG variable in /etc/sysconfig/i18n file
 */
 
char *natspec_get_system_locale();

/*
 * Returns const string with current charset
 */
const char *natspec_get_charset();



/* Returns static string with charset of _type_ op. system
 * for locale in form language[_territory][.charset][@modifier}
 * where language is an ISO 639 language code, territory
 * is an ISO 3166 country code, and codeset is a character set or encoding
 * identifier like ISO-8859-1 or UTF-8
 * If locale is empty string, use system locale
 * If locale does not contains charset, get it from charset of this locale
 */

const char * natspec_get_charset_by_locale(const int type,
	const char *locale);

/*
char *natspec_get_codepage_by_locale(const int type,
	const char *locale);
*/

/*
 * Specially for mount command
 * Returns new allocated string with enriched options
 * options is pointer to pointer at options string, we do free(*options) inside
 * after new string is allocated
 */
char * natspec_enrich_fs_options(const char* fs, char **options);

/*
 * Returns static string with charset in nls form (like in kernel)
 * of filename in system.
 * If G_FILENAME_ENCODING exists, use its value
 * If locale is NULL, use system locale
 */
 
const char * natspec_get_filename_encoding(const char * locale);

/*
 * from charset to codepage translator
 * from IBM866 -> 866
 * from cp1251 -> 1251
 */
const char *natspec_get_codepage_by_charset(const char *cs);

/* from KOI8R -> koi8-r */
const char *natspec_get_nls_by_charset(const char *cs);


/* Converts a string from one character set to another.
 * If tocode or fromcode is NULL, it assume as local charset
 * If the conversion was successful,
 * Returns: a newly allocated nul-terminated string,
 * 	which must be freed with free(). Otherwise NULL.
 */
char *natspec_convert(const char *in_str, const char *tocode, const char *fromcode);


/* Not recommended for use: */

/* 
 * Returns malloc allocated string with charset get from _locale_
 */
 
char *natspec_extract_charset_from_locale(const char *locale);

/* Removes punctuation characters from charset name */
char *natspec_humble_charset( const char *charset);


/*
 * Return static string with charset of _type_ op. system
 * if we know _charset_ for _bytype_ op. system
 */

const char * natspec_get_charset_by_charset(const int type,
	const int bytype, const char *charset);

#ifdef __cplusplus
}
#endif

#endif
