/*
    src/natspec.c
	
	command line util for access to libnatspec functions

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: natspec.c,v 1.13 2005/03/02 18:25:38 lav Exp $

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
#include <strings.h>
#include <assert.h>

#include "natspec_internal.h"

#ifdef HAVE_LIBPOPT
#  include <popt.h>
#endif


char *charset_type;
char *locale;
int version, verbose, fsenc, fcodepage, nls, flag_help, info, flag_locale;

#if defined HAVE_LIBPOPT
poptContext context = NULL;
struct poptOption options[] =
{
    {"charset", 's', POPT_ARG_STRING,  &charset_type, 0,
     "print charset for op. system: unix, win, dos, mac (case insensivity)", ""},
    {"locale", 'l', POPT_ARG_NONE,  &flag_locale, 1,
     "print user locale", ""},
    {"fsenc", 'f', POPT_ARG_NONE,  &fsenc, 1,
     "print filesystem encoding", ""},
/*    {"nls", 'n', POPT_ARG_NONE,  &nls, 1,
     "print filesystem encoding in nls form", ""},
*/
    {"info", 'i', POPT_ARG_NONE,  &info, 0,
     "print overall encoding/charset info for your system", ""},
    {"codepage", 'c', POPT_ARG_NONE,  &fcodepage, 1,
     "print codepage", ""},
    {"version", 'V', POPT_ARG_NONE, &version, 1,
     "display version and exit", NULL },
    {"verbose", 'v', POPT_ARG_NONE, &verbose, 0,
     "verbose output", NULL },
    {"help", 'h', POPT_ARG_NONE, &flag_help, 1, "Show this help message" },
    {(char *) NULL, '\0', 0, NULL, 0}
};
#endif

static const char *get_charset(char *charset_type)
{
	const char *charset;
	int type;
	assert (charset_type);
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
		printf("ASCII\n");
		exit(1);
	}
	charset = natspec_get_charset_by_locale(type, locale);
	if (verbose)
		printf("Charset of '%s' system: %s\n", charset_type, charset);
	else
		printf("%s\n",charset);
	return charset;
}


int main(int argc, const char** argv)
{

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
	printf("Compiled without popt\n");
	exit(1);
#endif
	if (argc == 1)
		info = 1;
	if (version || info)
	{
		printf("%s version %s\n",PACKAGE, PACKAGE_VERSION);
		if (!info) exit(0);
	}
	if (info)
	{
		printf(" === Overall information ===\n");
		verbose = 1;
	}
	locale = natspec_get_user_locale();
	if (flag_locale || info)
	{
		if (verbose) printf("Using (user) locale: ");
		printf("%s\n",locale);
	}
	if (verbose)
		printf("System locale: %s\n", natspec_get_system_locale());
	if (fsenc || info)
	{
		const char *buf;
		if (verbose) printf("Filename encoding in iconv/nls form: ");
		buf = natspec_get_filename_encoding("");
		printf("%s\n",buf);
		if (!info) exit(0);
	}
/*	if (nls || info)
	{
		const char *buf;
		if (verbose) printf("Filename system encoding in nls form (iocharset): ");
		buf = natspec_get_filename_encoding(locale);
		printf("%s\n",buf);
		if (!info) exit(0);
	}
*/
	if (fcodepage || info)
	{
		const char *buf;
		if (verbose) printf("Codepage of DOS: ");
		buf = natspec_get_codepage_by_charset(
			natspec_get_charset_by_locale(NATSPEC_DOSCS, locale));
		printf("%s\n",buf);
		if (!info) exit(0);
	}
	if (info)
	{
		char *types[]={"UNIX","WIN","DOS","MAC"};
		int i;
		for (i=0; i<sizeof(types)/sizeof(char*);i++)
			get_charset(types[i]);
		exit(0);
	}
	if (charset_type)
		get_charset(charset_type);
	return 0;
}
