/*
    src/natspec.c
	
	command line util for access to libnatspec functions

    NATSPEC library
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
#include <string.h>
#include "natspec.h"
#include "../config.h"

#ifdef HAVE_LIBPOPT
#  include <popt.h>
#endif


char *charset_type;
char *locale;
int version, verbose, fsenc, fcodepage, nls, flag_help;

#if defined HAVE_LIBPOPT
poptContext context = NULL;
struct poptOption options[] =
{
    {"type", 't', POPT_ARG_STRING,  &charset_type, 0,
     "Type of needed charset: unix (default), win, dos, mac", ""},
    {"locale", 'l', POPT_ARG_STRING,  &locale, 0,
     "Using locale (from $LANG by default)", ""},
    {"fsenc", 'f', POPT_ARG_NONE,  &fsenc, 0,
     "Get filesystem encoding", ""},
    {"nls", 'n', POPT_ARG_NONE,  &nls, 0,
     "Get filesystem encoding in nls form", ""},
    {"codepage", 'c', POPT_ARG_NONE,  &fcodepage, 0,
     "Get codepage", ""},
    {"version", 'V', POPT_ARG_NONE, &version, 0,
     "Display version and exit", NULL },
    {"verbose", 'v', POPT_ARG_NONE, &verbose, 0,
     "Verbose output", NULL },
    {"help", 'h', POPT_ARG_NONE, &flag_help, 1, "Show this help message" },
    {(char *) NULL, '\0', 0, NULL, 0}
};
#endif

int main(int argc, const char** argv)
{
	const char *charset;
	int type;

#if defined HAVE_LIBPOPT
  int rc = 0;
  poptContext poptCtx;
  poptCtx = poptGetContext("natspec", argc, (const char **)argv, options, 0);
  poptSetOtherOptionHelp(poptCtx, "[OPTION...]");
  while (rc >= 0) {
    if((rc = poptGetNextOpt(poptCtx)) < -1) {
      printf(("Error on option %s: %s.\nRun '%s --help' to see a full list of available command line options.\n"),
	      poptBadOption(poptCtx, 0),
	      poptStrerror(rc),
	      argv[0]);
      exit(1);
    }
    if(rc == 1) {
    }
  }
/*
  { int flag_help=1;
  if (argv) {
      while (poptPeekArg(poptCtx))
		flag_help=0;
		//locale = (char *)poptGetArg(poptCtx);
      poptFreeContext(poptCtx);
	}
*/	
    if(flag_help)
	{
	poptPrintHelp(poptCtx, stderr, 0);
    exit(0);
	}

#else
	printf("Complied without popt\n");
	exit(1);
#endif
	if (version)
	{
		printf("Test version of natspec\n");
		exit(0);
	}
	if (!locale)
	{
		if (verbose) printf("Use $LANG as locale variable\n");
		locale=getenv("LANG");
	}
	if (fsenc)
	{
		const char *buf;
		if (verbose) printf("Filename encoding:\n");
		buf = natspec_get_filename_encoding(locale);
		printf("%s\n",buf);
		exit(0);
	}
	if (nls)
	{
		const char *buf;
		if (verbose) printf("Filename encoding in nls form:\n");
		buf = natspec_get_nls_from_charset(
			natspec_get_filename_encoding(locale));
		printf("%s\n",buf);
		exit(0);
	}
	if (fcodepage)
	{
		const char *buf;
		if (verbose) printf("Codepage:\n");
		buf = natspec_get_codepage_from_charset(
			natspec_get_charset_by_locale(NATSPEC_DOSCS, locale));
		printf("%s\n",buf);
		exit(0);
	}
	if (!charset_type || !charset_type[0])
		charset_type="UNIX";
	if (!strcasecmp(charset_type,"WIN"))
		type = NATSPEC_WINCS;
	else if (!strcasecmp(charset_type,"DOS"))
		type = NATSPEC_DOSCS;
	else if (!strcasecmp(charset_type,"MAC"))
		type = NATSPEC_MACCS;
	else if (!strcasecmp(charset_type,"UNIX"))
		type = NATSPEC_UNIXCS;
	else
	{
		if (verbose) printf("We do not know %s type of encoding\n",charset_type);
		exit(1);
	}
		charset = natspec_get_charset_by_locale(type, locale);
	if (verbose)
		printf("locale:%s, type:%s, charset:%s\n",locale,charset_type,charset);
	else
		printf("%s\n",charset);
}
