/* natspec library header file
 * Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

/* Types of character set tables for various OS */

#define NATSPEC_UNIXCS 0
#define NATSPEC_WINCS 1
#define NATSPEC_DOSCS 2
#define NATSPEC_MACCS 3

/* 
 * Returns malloc allocated string with system locale
 * get from LANG variable in /etc/sysconfig/i18n file
 */
 
char *natspec_get_system_locale();


/*
 * Returns static string with charset of filename
 * in system
 * If G_FILENAME_ENCODING exists, returns its value
 * If locale is empty string, use system locale
 */
 
const char * natspec_get_filename_encoding(const char * locale);


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


/* Not recommended for use: */


/* 
 * Returns malloc allocated string with charset get from _locale_
 */
 
char *natspec_get_charset_from_locale(const char *locale);


/*
 * Return static strig with charset of _type_ op. system
 * if we know _charset_ for _bytype_ op. system
 */

const char * natspec_get_charset_by_charset(const int type,
	const int bytype, const char *charset);

