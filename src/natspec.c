/*
 * Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

#include <stdlib.h>
#include <string.h>
#include "natspec.h"


#define HAVE_POPT_H
#define HAVE_LIBPOPT

#if (defined (HAVE_LIBPOPT) && defined (HAVE_POPT_H))
#define HAVE_POPT
#endif




#ifdef HAVE_POPT_H
#  include <popt.h>
#endif


char *charset_type;
char *locale;
int version, verbose, fsenc;

#if defined HAVE_POPT
poptContext context = NULL;
struct poptOption options[] =
{
    {"type", 't', POPT_ARG_STRING,  &charset_type, 0,
     "Type of needed charset: unix (default), win, dos, mac", ""},
    {"locale", 'l', POPT_ARG_STRING,  &locale, 0,
     "Using locale (from $LANG by default)", ""},
    {"fsenc", 'f', POPT_ARG_NONE,  &fsenc, 0,
     "Get filesystem encoding", ""},
    {"version", 'V', POPT_ARG_NONE, &version, 0,
     "Display version and exit", NULL },
    {"verbose", 'v', POPT_ARG_NONE, &verbose, 0,
     "Verbose output", NULL },
    {"help", 'h', POPT_ARG_NONE, 0, 1, "Show this help message" },
    {(char *) NULL, '\0', 0, NULL, 0}
};
#endif

int main(int argc, const char** argv)
{
	const char *charset;
	int type;

#if defined HAVE_POPT
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
  if (argv) {
      while (poptPeekArg(poptCtx))
		locale = (char *)poptGetArg(poptCtx);
      poptFreeContext(poptCtx);
	}
	*/
    if(argc == 1)
	{
	poptPrintHelp(poptCtx, stderr, 0);
    exit(0);
	}

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
		buf = natspec_get_filename_encoding(locale);
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
