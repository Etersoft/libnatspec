/* Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

/* Returns static string with charset
 * for locale in form language[_territory][.charset][@modifier}
 * where language is an ISO 639 language code, territory
 * is an ISO 3166 country code, and codeset is a character set or encoding
 * identifier like ISO-8859-1 or UTF-8
 * If locale does not contains charset, pr... it from charset this locale
 */

/*
// or NULL if it is not defined
const char * natspec_get_unix_charset(const char *locale);
const char * natspec_get_win_charset(const char *locale);
const char * natspec_get_dos_charset(const char *locale);
const char * natspec_get_mac_charset(const char *locale);
*/

#define NATSPEC_UNIXCS 0
#define NATSPEC_WINCS 1
#define NATSPEC_DOSCS 2
#define NATSPEC_MACCS 3

const char * natspec_get_charset_by_locale(const int type,
	const char *locale);

const char * natspec_get_charset_by_charset(const int type,
	const int bytype, const char *unixcs);

const char * natspec_get_filename_encoding(const char * locale);
