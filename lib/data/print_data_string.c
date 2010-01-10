/* Helper for autogenerate charset_relation table
    $Id: print_data_string.c,v 1.10 2005/03/17 15:29:17 lav Exp $
*/

#include <windows.h>
#include <winnls.h>
//#include <wine/unicode.h>

#include <stdlib.h>
#include <stdio.h>

#include "charset_names.h"
# include <langinfo.h>

static const char * get_charset (UINT codepage)
{
  int i;
  for (i = 0; i < sizeof (charset_names) / sizeof (charset_names[0]); i++)
    {
      if (charset_names[i].codepage == codepage)
	{
	  return charset_names[i].charset_name;
	}
    }
	fprintf(stderr,"Can't locate codepage %d for %s locale\n",codepage,getenv("LANG"));
  return "";
}

/* Next three func copied from libnatspec / get_charset.c and get_locale.c */

/* Removes punctuation characters from charset name */
char *natspec_humble_charset( const char *charset)
{
	int i, j;
	char *buf;
	if (!charset || !charset[0])
		return NULL;
	/* Do not remove punctuation for ANSI encoding... */
	if (strstr(charset,"ANSI_X3") != NULL)
		return (char*)charset;
	buf = malloc( strlen(charset) + 1 );
   	for (i = 0, j = 0; charset[i]; i++)
        if (isalnum(charset[i]))
			buf[j++] = toupper(charset[i]);
   	buf[j] = 0;
	return buf;
}

/* Returns charset get from _locale_ */
char *natspec_extract_charset_from_locale(const char *locale)
{
	char *lang, *next, *dialect, *charset, *ret;
    if (locale == NULL || locale[0] == '\0')
		return NULL;
	lang = strdup( locale );
    next = strchr(lang,':'); if (next) *next++ = '\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++ = '\0';
    charset = strchr(lang,'.'); if (charset) *charset++ = '\0';
	ret = natspec_humble_charset(charset);
	free (lang);
	return ret;
}


/* Internal: repack locale string (compress charset, fix register) */
char *_natspec_repack_locale(const char *locale)
{
	int i;
	char *buf, *lang, *next, *dialect, *charset, *country;
    if (!locale || !locale[0])
		return NULL;
	lang = strdup( locale );
	buf = malloc( strlen(locale) + 1 );
    next = strchr(lang,':'); if (next) *next++ = '\0';
    dialect = strchr(lang,'@'); if (dialect) *dialect++ = '\0';
    charset = strchr(lang,'.'); if (charset) *charset++ = '\0';
    country = strchr(lang,'_'); if (country) *country++ = '\0';

	for (i=0; i<strlen(lang); i++)
		tolower(lang[i]);
	strcpy(buf, lang);
	if (country)
	{
		for (i=0; i<strlen(country); i++)
			toupper(country[i]);
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
		for(i=0; i<strlen(dialect); i++)
			tolower(dialect[i]);
		strcat(buf, "@");
		strcat(buf, dialect);
	}
	free (lang);
	return buf;
}

void print_w( char *str, int w)
{
  char buf[100]; int i;
  sprintf(buf, "%s%s", str, ( w ? ", " : ""));
  printf("%s", buf);
  i=w-strlen(buf);
  if (i>0)
	  for (;i;i--)
  		printf(" ");
}

void print_charset( UINT cp, int w, char *tt)
{
  char buf[100];
  const char *t = tt;
  if (cp)
  	t=get_charset (cp);
  sprintf (buf,"\"%s\"", t);
  if (strlen(t))
	  print_w(buf, w);
	else
		print_w(" NULL",w);
}

int
main (int argc, char **argv)
{
 /* Or system? */
  LCID lcid = GetUserDefaultLCID ();
  UINT ansi_cp, mac_cp, oem_cp, unix_cp;
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTUNIXCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & unix_cp, sizeof (unix_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & ansi_cp, sizeof (ansi_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & oem_cp, sizeof (oem_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTMACCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & mac_cp, sizeof (mac_cp) / sizeof (WCHAR));
	char buf[100];
	char *l = _natspec_repack_locale(getenv("LANG"));
	sprintf(buf,"\"%s\"",l);
  print_w (buf, 16);
	
  sprintf (buf, "%d", lcid);
  print_w (buf, 7);
  /* I don't know how to set wine for system enc */
#ifdef DUMMY
  print_charset(unix_cp, 15,"");
#else
  print_charset(0,15,natspec_humble_charset(nl_langinfo(CODESET)));
#endif

  print_charset(ansi_cp, 12, "");
  print_charset(oem_cp, 12, "");
  print_charset(mac_cp, 0, "");
  return 0;
}
