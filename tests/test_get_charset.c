/*
 * Public domain
 * Copyright (c) 2005 Etersoft
 * Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>
 */

#include <stdlib.h>
#include <locale.h>
#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "natspec.h"
#include "data/charset_names.h"

char *test_ee(char **t)
{
	char *ret;
	ret = natspec_enrich_fs_options("reiserfs", t);
	printf("enrich reiserfs: %s\n",ret);
	return ret;

}

void test_for_enrich()
{
	char *ret, *ret1;
	ret = NULL;
	ret = natspec_enrich_fs_options(NULL, NULL);
	printf("enrich NULL pointer: %s\n", ret);
	ret = NULL;
	ret1 = ret;
	natspec_enrich_fs_options(NULL, &ret);
	printf("enrich NULL: %s\n",ret);
	assert (ret == ret1);
	ret1 = ret;
	natspec_enrich_fs_options("udf", &ret);
	printf("enrich udf: ret=%s, ret1=%s\n",ret, ret1);
	assert (ret != ret1);

	ret1 = ret = strdup("defaults");
	natspec_enrich_fs_options("vfat", &ret);
	printf("enrich VFAT: %s %p:%p\n",ret,ret,ret1);
	assert (ret != ret1);

	ret1 = ret = strdup("");
	natspec_enrich_fs_options("smb", &ret);
	printf("enrich SMB: %s %p:%p\n",ret,ret,ret1);
	assert (ret != ret1);

	ret = strdup("defaults");
	ret1 = ret;
	test_ee(&ret);
	assert (ret == ret1);
}

void test_for_iconv()
{
  int i;
  for (i = 0; i < sizeof (charset_names) / sizeof (charset_names[0]); i++)
    {
	  const char *cs = charset_names[i].charset_name;
	  iconv_t it;
	  it = iconv_open(cs,"UTF-8");
	  if ( it == (iconv_t)(-1))
	  {
	  	printf("TEST_ICONV: does not know '%s'\n",cs);
		exit (1);
		}
	else
		iconv_close(it);
    }
	printf("TEST_ICONV: comformance test is completed\n");
}

int main(void)
{
	int i;
	char *locale[6];
	locale[0] = getenv("LANG");
	locale[1] = getenv("LC_CTYPE");
	locale[2] = "";
	locale[3] = NULL;
	locale[4] = "ru";
	locale[5] = "ru_RU";
	for (i=0; i<sizeof(locale)/sizeof(char*); i++)
	{
		printf("locale:%s\n",locale[i]);
		printf("\tunix=%s\n\twin=%s\n\tdos=%s\n\tmac=%s\n",
			natspec_get_charset_by_locale(NATSPEC_UNIXCS,locale[i]),
			natspec_get_charset_by_locale(NATSPEC_WINCS,locale[i]),
			natspec_get_charset_by_locale(NATSPEC_DOSCS,locale[i]),
			natspec_get_charset_by_locale(NATSPEC_MACCS,locale[i])
		);
	}
	printf("charset:%s\n",	natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS,NULL));
	printf("fileenc:%s\n",natspec_get_filename_encoding (""));
	printf("system locale:%s\n",natspec_get_system_locale ());
	test_for_iconv();
	test_for_enrich();
	return 0;
}
