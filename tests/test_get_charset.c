#include <stdlib.h>
#include "get_charset.h"
#include <locale.h>

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
	//setlocale(LC_ALL,"");
	printf("charset:%s\n",	natspec_get_charset_by_charset (NATSPEC_UNIXCS,NATSPEC_UNIXCS,NULL));
	printf("fileenc:%s\n",natspec_get_filename_encoding (""));
	return 0;
}
