/*
 * Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 *
 * $Id: test_get_charset.c,v 1.17 2006/01/03 01:26:15 vitlav Exp $
 *
 */

#include <stdlib.h>
#include <locale.h>
/* #include <iconv.h> */
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "natspec_internal.h"
#include "data/charset_names.h"
#include "data/get_charset_data.h"

void test_for_enrich()
{
	char *ret, *ret1;
	ret1 = natspec_get_enriched_fs_options(NULL, NULL);
	printf("enrich NULL: %s\n",ret1);
	ret = NULL;
	ret1 = natspec_get_enriched_fs_options("udf", ret);
	printf("enrich udf: ret='%s', ret1='%s'\n",ret, ret1);

	ret = ",";
	ret1 = natspec_get_enriched_fs_options("udf", ret);
	printf("enrich udf: ret='%s', ret1='%s'\n",ret, ret1);
	assert (strcmp(ret,ret1));

	ret = "defaults";
	ret1 = natspec_get_enriched_fs_options("vfat", ret);
	printf("enrich VFAT: %s %p:%p\n",ret,ret,ret1);
	assert (strcmp(ret,ret1));

	ret = "iocharset=koi8-r";
	ret1 = natspec_get_enriched_fs_options("vfat", ret);
	printf("enrich VFAT with io: %s %p:%p\n",ret,ret,ret1);
	assert (!strcmp(ret,ret1));

	ret = "";
	ret1 = natspec_get_enriched_fs_options("smb", ret);
	printf("enrich SMB: %s\n",ret1);
	assert (strcmp(ret,ret1));

	ret = strdup("defaults");
	ret1 = natspec_get_enriched_fs_options("reiserfs", ret);
	printf("enrich reiserfs: %s\n",ret1);
	assert (!strcmp(ret,ret1));
}

void test_for_iconv()
{
	int i;
	for (i = 0; i < sizeof (charset_names) / sizeof (charset_names[0]); i++) {
		const char *cs = charset_names[i].charset_name;
		iconv_t it;
		it = iconv_open(cs,"UTF-8");
		if ( it == (iconv_t)(-1)) {
			printf("TEST_ICONV: does not know '%s'\n",cs);
			exit (1);
		}
		else
			iconv_close(it);
	}
	printf("TEST_ICONV: comformance test is completed: %d charsets\n",i);
}

void test_for_convert()
{
	iconv_t it; size_t li, lo, result;
	char *t, *ti;
	char toi[100],to[100], *tob, *toai;
	ti = "Test –“œ◊≈“À¡";
	t = natspec_convert(ti, "ASCII", NULL, 1);
	printf("1 %s -> '%s'\n",ti,t);
	t = natspec_convert(ti, "UTF8", "", 1);
	printf("2 %s -> '%s'\n",ti,t);
	ti = natspec_convert(t, "", "UTF8", 1);
	printf("3 %s -> '%s'\n",t,ti);
	ti = "œÓ‚ÂÍ‡";
	t = natspec_convert(ti, "", "CP1251", 1);
	printf("%s -> '%s'\n",ti,t);
	ti = "œÓ‚ÂÍ‡";
	t = natspec_convert(ti, "", "CP1251", 0);
	printf("%s -> '%s'\n",ti,t);
	ti = "œÓ‚ÂÍ‡ \xb9";
	t = natspec_convert(ti, "", "CP1251", 0);
	printf("%s -> '%s'\n",ti,t);
	
	it = natspec_iconv_open("aSCII", "");
	if ( it == (iconv_t)(-1)) {
		printf("natspec_iconv: some problems\n");
		exit (1);
	}
	strcpy(toi,"Test - “œ◊≈“À¡ £ÃœﬁŒŸ» …«“’€≈À.");
	li = strlen(toi); lo = 99; tob = to; toai = toi;
	printf("Before natspec_iconv: it=%d, %s, len=%d\n", it, toi, li);
	result = natspec_iconv(it, &toai, &li, &tob, &lo, 1);
	*tob = '\0';
	printf("Result natspec_iconv: %s (lo=%d), with result=%d\n", to, lo, result);
	iconv_close(it);
}

void test_nls()
{
	int i;
	/* All locales */
	for (i = 0; i< sizeof(charset_relation)/sizeof(charset_relation[0]); i++)
	{
		const char *t;
		iconv_t it;
		const char *locale = charset_relation[i].locale;
		assert (t=natspec_get_filename_encoding(locale));
		/* printf("%s\n",t); */
		it = iconv_open(t,"UTF-8");
		if ( it == (iconv_t)(-1))
	  	{
		  	printf("TEST_ICONV for nls: does not know '%s'\n",t);
			exit (1);
		}
		else
			iconv_close(it);
		t = natspec_get_charset_by_locale(NATSPEC_UNIXCS, locale);
		setlocale(LC_ALL,locale);
		if(natspec_get_charset() != t)
			printf("Some problem in setting locale: %s CS: %s - from table: %s\n",locale, natspec_get_charset(),t);
	}
	printf("TEST_NLS: comformance test is completed: %d locales\n",i);
}


#define ok(f,n) printf("%s == %s: %s\n", f,n, (!strcmp(f,n)) ? "PASSED" : "failed");
#define okn(f,n) printf("%s == %s: %s\n", f,n, (f == n) ? "PASSED" : "failed");

void compliant_test()
{
	ok(natspec_get_filename_encoding ("ru_RU.UTF8"), "utf8");
	ok(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, "UTF-8"), "utf8");
	okn(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, ""), NULL);
	okn(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, "haha"), NULL);
	okn(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, NULL), "koi8-r");
}

int main(void)
{
	int i;
	char *locale[7];
	const char *cas;
	locale[0] = getenv("LANG");
	locale[1] = "POSIX";
	locale[2] = "C";
	locale[3] = NULL;
	locale[4] = "ru";
	locale[5] = "ru_RU";
	locale[6] = "";
	setlocale(LC_ALL,"");
	printf("FIRST current charset (nl):%s\n",	natspec_get_charset ());
	printf("FIRST fileenc: %s\n",natspec_get_filename_encoding (""));
	printf("user locale:%s\n",locale[0]);
	printf("current locale:%s\n",natspec_get_current_locale ());
	printf("system locale:%s\n",natspec_get_system_locale ());
	printf("current charset (nl):%s\n",	natspec_get_charset ());
	cas = natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS,NULL);
	printf("current charset:%s\n",cas);
	/*assert (!strcmp(cas, natspec_get_charset()));*/
	
	for (i=0; i<sizeof(locale)/sizeof(char*); i++)
	{
		int j;
		printf("locale:'%s'\n",locale[i]);
		for (j = NATSPEC_UNIXCS; j<=NATSPEC_MACCS; j++)
		{
			const char *e = natspec_get_charset_by_locale(j,locale[i]);
			printf ("\tfor %d CS: '%s' (%s)\n", j, e, natspec_get_nls_by_charset(e));
		}
	}
	printf("fileenc: %s\n",natspec_get_filename_encoding (""));
	test_for_iconv();
	test_nls();
	test_for_enrich();
	printf("SECOND current charset (nl):%s\n",	natspec_get_charset ());
	cas = setlocale(LC_ALL, "");
	printf("old locale: %s\n",cas);
	test_for_convert();
	compliant_test();
	return 0;
}
