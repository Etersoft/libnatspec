#include <windows.h>
#include <winnls.h>
//#include <wine/unicode.h>

#include <stdlib.h>
#include <stdio.h>

#include "charset_names.h"

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
	fprintf(stderr,"Can't locate codepage %d\n",codepage);
  return "";
}

int
main (int argc, char **argv)
{

  LCID lcid = GetSystemDefaultLCID ();
  UINT ansi_cp, mac_cp, oem_cp, unix_cp;
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTUNIXCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & unix_cp, sizeof (unix_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTANSICODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & ansi_cp, sizeof (ansi_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & oem_cp, sizeof (oem_cp) / sizeof (WCHAR));
  GetLocaleInfoW (lcid, LOCALE_IDEFAULTMACCODEPAGE | LOCALE_RETURN_NUMBER,
		  (LPWSTR) & mac_cp, sizeof (mac_cp) / sizeof (WCHAR));

  //printf ("acp=%d mac=%d oem=%d unix=%d\n",ansi_cp, mac_cp, oem_cp, unix_cp);
  printf ("%d ", lcid);
  printf ("%s ", get_charset (unix_cp));
  printf ("%s ", get_charset (ansi_cp));
  printf ("%s ", get_charset (oem_cp));
  printf ("%s ", get_charset (mac_cp));
  return 0;
}
