#include <windows.h>
#include <winnls.h>
#include <wine/unicode.h>

#include <stdlib.h>
#include <stdio.h>

#include "charset_names.h"

char charset_name[100];
static char *
get_charset (UINT codepage, char *str)
{
  int i;
  for (i = 0; i < sizeof (charset_names) / sizeof (charset_names[0]); i++)
    {
      if (charset_names[i].codepage == codepage)
	{
	  strcpy (charset_name + 1, charset_names[i].charset_name);
	  charset_name[0] = '"';
	  strcat (charset_name, "\"");
	  strcat (charset_name, str);
	  return charset_name;
	}
    }
  strcpy (charset_name, "\"\"");
  strcat (charset_name, str);
  return charset_name;
}

int
main (int argc, char **argv)
{
  // the current original equipment manufacturer (OEM) code-page identifier for the system.
  UINT oemcp = GetOEMCP ();
  const union cptable *t;
  char buf[100];

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
  sprintf (buf, "%d", lcid);
  strcat (buf, ",");
  printf ("%-8s ", buf);
  printf ("%-10s ", get_charset (unix_cp, ","));
  printf ("%-10s ", get_charset (ansi_cp, ","));
  printf ("%-10s ", get_charset (oem_cp, ","));
  printf ("%-10s ", get_charset (mac_cp, ""));
  return 0;
}
