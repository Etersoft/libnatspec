/*
 * Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 *
 * $Id: test_get_charset.c,v 1.21 2008/02/14 23:09:36 vitlav Exp $
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
	free(ret1);

	ret = NULL;
	ret1 = natspec_get_enriched_fs_options("udf", ret);
	printf("enrich udf: ret='%s', ret1='%s'\n",ret, ret1);
	free(ret1);

	ret = ",";
	ret1 = natspec_get_enriched_fs_options("udf", ret);
	printf("enrich udf: ret='%s', ret1='%s'\n",ret, ret1);
	assert (strcmp(ret,ret1));
	free(ret1);

	ret = "defaults";
	ret1 = natspec_get_enriched_fs_options("vfat", ret);
	printf("enrich VFAT: %s %p:%p\n",ret,ret,ret1);
	assert (strcmp(ret,ret1));
	free(ret1);

	ret = "iocharset=koi8-r";
	ret1 = natspec_get_enriched_fs_options("vfat", ret);
	printf("enrich VFAT with io: %s %p:%p\n",ret,ret,ret1);
	assert (!strcmp(ret,ret1));
	free(ret1);

	ret = "";
	ret1 = natspec_get_enriched_fs_options("smb", ret);
	printf("enrich SMB: %s\n",ret1);
	assert (strcmp(ret,ret1));
	free(ret1);

	ret = strdup("defaults");
	ret1 = natspec_get_enriched_fs_options("reiserfs", ret);
	printf("enrich reiserfs: %s\n",ret1);
	assert (!strcmp(ret,ret1));
	free(ret);
	free(ret1);
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
	ti = "Test “œ◊≈“À¡";
	t = natspec_convert(ti, "IBM866", "KOI8R", 0);
	printf("%s -> '%s'\n",ti,t);
	
	it = natspec_iconv_open("aSCII", "");
	if ( it == (iconv_t)(-1)) {
		printf("natspec_iconv: some problems\n");
		exit (1);
	}
	strcpy(toi,"Test - “œ◊≈“À¡ £ÃœﬁŒŸ» …«“’€≈À.");
	li = strlen(toi); lo = 99; tob = to; toai = toi;
	printf("Before natspec_iconv: it=%p, %s, len=%lu\n", it, toi, li);
	result = natspec_iconv(it, &toai, &li, &tob, &lo, 1);
	*tob = '\0';
	printf("Result natspec_iconv: %s (lo=%lu), with result=%lu\n", to, lo, result);
	natspec_iconv_close(it);
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
		t = natspec_get_filename_encoding(locale);
		assert (t);
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
#define okn(f,n) printf("%s == NULL: %s\n", f, (f == NULL) ? "PASSED" : "failed");

void compliant_test()
{
/*
	printf("%s\n", natspec_get_charset_by_locale(NATSPEC_UNIXCS, "ru_RU.UTF8"));
	printf("%s\n", natspec_get_nls_by_charset("UTF8"));
*/
	/* FIXME: can be overrided by G_FILENAME_ENCODING */
	ok(natspec_get_filename_encoding ("ru_RU.UTF8"), "utf8");
	ok(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, "UTF-8"), "UTF8");
	ok(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, ""), "KOI8R");
	okn(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, "haha"), NULL);
	ok(natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS, NULL), "KOI8R");
}

char tin[]={0x4,0x22,0,0};

void test_for_natspec_iconv()
{
	/*char *inbuf="“…◊≈‘!";*/
	char *inbuf=tin;
	/*int insize=strlen(inbuf);*/
	size_t insize=2, size;
	char buf[100]; size_t outsize = 99;
	char *outbuf = buf;
	iconv_t tt = natspec_iconv_open("koi8-r","UCS-2BE");
	assert (tt != (iconv_t)-1);
	size = natspec_iconv(tt, &inbuf, &insize, &outbuf, &outsize, 0);
	*outbuf = 0;
	printf("natspec_iconv: insize=%lu outsize=%lu size=%lu outbuf=%s\n", insize, outsize, size, buf);
}

int main(void)
{
	int i;
	char *locale[8];
	const char *cas;
	locale[0] = getenv("LANG");
	locale[1] = "POSIX";
	locale[2] = "C";
	locale[3] = NULL;
	locale[4] = "ru";
	locale[5] = "ru_RU";
	locale[6] = "";
	locale[7] = "ru_RU.UTF8";
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
	test_for_natspec_iconv();
	compliant_test();
	printf("DONE\n");
	return 0;
}
