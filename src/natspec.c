/*
    src/natspec.c
	
	command line util for access to libnatspec functions

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: natspec.c,v 1.22 2005/09/03 18:59:28 vitlav Exp $

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
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <locale.h>

#include "natspec_internal.h"
#include "natspechi_internal.h"

#ifdef HAVE_LIBPOPT
#  include <popt.h>
#endif

#include <locale.h>
#ifdef HAVE_LANGINFO_H
# include <langinfo.h>
#endif


char *charset_type;
char *locale, *transliterate;
int version, verbose, fsenc, fcodepage, nls, flag_help, info;
int flag_locale, utf8, country_id, get_system_locale;

#if defined HAVE_LIBPOPT
poptContext context = NULL;
struct poptOption options[] =
{
    {"codepage", 'c', POPT_ARG_NONE,  &fcodepage, 1,
     "print codepage", ""},
    {"fsenc", 'f', POPT_ARG_NONE,  &fsenc, 1,
     "print filesystem encoding", ""},
	{"utf8", 0, POPT_ARG_NONE,  &utf8, 1,
     "return 0 if locale is utfed", ""},
	{"country-id", 0, POPT_ARG_NONE,  &country_id, 1,
     "print country ID (international numbering)", ""},
	{"system-locale", 0, POPT_ARG_NONE,  &get_system_locale, 1,
     "print system locale", ""},
    {"info", 'i', POPT_ARG_NONE,  &info, 0,
     "print overall encoding/charset info for your system", ""},
    {"transl", 'a', POPT_ARG_STRING,  &transliterate, 1,
     "transliterate args from ENC to current encoding and print it", ""},
    {"version", 'V', POPT_ARG_NONE, &version, 1,
     "display version and exit", NULL },
    {"verbose", 'v', POPT_ARG_NONE, &verbose, 0,
     "verbose output", NULL },
    {"charset", 's', POPT_ARG_STRING,  &charset_type, 0,
     "print charset for op. system: unix, win, dos, mac (case insensivity)", ""},
    {"help", 'h', POPT_ARG_NONE, &flag_help, 1, "Show this help message" },
    {(char *) NULL, '\0', 0, NULL, 0}
};
#endif

static const char *get_charset(char *charset_type)
{
	/* FIXME: strcasecmp is locale depends */
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
	int is_utf8;
#if defined HAVE_LIBPOPT
	int rc = 0;
	poptContext poptCtx;
	poptCtx = poptGetContext("natspec", argc, (const char **)argv, options, 0);
	poptSetOtherOptionHelp(poptCtx, "[OPTION...]");
	while (rc >= 0) {
		if((rc = poptGetNextOpt(poptCtx)) < -1) {
			fprintf(stderr, "Error on option %s: %s.\nRun '%s --help' to see a full list of available command line options.\n",
				poptBadOption(poptCtx, 0),
				poptStrerror(rc),
				argv[0]);
			exit(1);
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
	poptPrintHelp(poptCtx, stdout, 0);
    exit(0);
	}

#else
	fprintf(stderr, "Compiled without popt. Exit\n");
	exit(1);
#endif
	if (argc == 1)
		info = 1;
	if (version || info)
	{
		printf("%s, compiled %s\n",PACKAGE_STRING,__DATE__);
		if (!info) exit(0);
	}
	if (transliterate)
	{
		char *str = NULL;
		if (argv) {
			while (poptPeekArg(poptCtx))
				str = (char *)poptGetArg(poptCtx);
			poptFreeContext(poptCtx);
		}
		if ( str != NULL)
		{
			setlocale(LC_ALL,"");
			str = natspec_convert_with_translit(str, "", transliterate);
			puts(str);
		}
		exit(0);
	}
	if (info)
	{
		printf(" === Overall information ===\n");
		verbose = 1;
	}
	locale = natspec_get_current_locale();
	if (flag_locale || info)
	{
		if (verbose) printf("Current locale: ");
		printf("%s",locale);
		if (verbose) puts("");
	}
	if (get_system_locale || verbose)
	{
		if (verbose) printf("System locale: ");
		printf("%s", natspec_get_system_locale());
		if (verbose) puts("");
		if (!info) exit(0);
	}
	if (fsenc || info)
	{
		const char *buf;
		if (verbose) printf("Filename encoding in iconv/nls form: ");
		buf = natspec_get_filename_encoding("");
		printf("%s",buf);
		if (verbose) puts("");
		if (!info) exit(0);
	}
	if (country_id || info)
	{
		int id;
		if (verbose) printf("Country ID (for DOS): ");
		id = -1; /* natspec_get_country_id(); */
		printf("%d", id);
		if (verbose) puts("");
		if (!info) exit(0);
	}
	if (fcodepage || info)
	{
		int cp;
		if (verbose) printf("Codepage of DOS: ");
		cp = natspec_get_codepage_by_charset(
			natspec_get_charset_by_locale(NATSPEC_DOSCS, locale));
		printf("%d", cp);
		if (verbose) puts("");
		if (!info) exit(0);
	}
	if (info)
	{
		char *types[]={"UNIX","WIN","DOS","MAC"};
		int i;
		for (i=0; i<sizeof(types)/sizeof(char*);i++)
			get_charset(types[i]);
		setlocale(LC_ALL,"");
		printf("After setlocale:\n");
#ifdef HAVE_LANGINFO_H
		printf("\tnl_langinfo(CODESET): %s\n",nl_langinfo(CODESET));
#endif
		printf("\tnatspec_get_charset: %s\n",natspec_get_charset());
	}
	if (charset_type)
		get_charset(charset_type);
	is_utf8 = natspec_locale_is_utf8("");
	if (verbose)
		printf("Current locale is%sin UTF8 encoding\n",(is_utf8 ? " " : " NOT "));
	if (utf8 && !info)
		return is_utf8 ? 0 : 1;
	return 0;
}
