/* This code contains charset to codepage map originally from
    WINE project (file dlls/kernel/locale.c)
	LGPL license
*/
/* 

TODO:
 - some gcc option for pack a static string?
 - add comments


from kernel fs/nls/Config.in
  string 'Default NLS Option' CONFIG_NLS_DEFAULT "iso8859-1"
  tristate 'Codepage 437 (United States, Canada)'  CONFIG_NLS_CODEPAGE_437
  tristate 'Codepage 737 (Greek)'                  CONFIG_NLS_CODEPAGE_737
  tristate 'Codepage 775 (Baltic Rim)'             CONFIG_NLS_CODEPAGE_775
  tristate 'Codepage 850 (Europe)'                 CONFIG_NLS_CODEPAGE_850
  tristate 'Codepage 852 (Central/Eastern Europe)' CONFIG_NLS_CODEPAGE_852
  tristate 'Codepage 855 (Cyrillic)'               CONFIG_NLS_CODEPAGE_855
  tristate 'Codepage 857 (Turkish)'                CONFIG_NLS_CODEPAGE_857
  tristate 'Codepage 860 (Portuguese)'             CONFIG_NLS_CODEPAGE_860
  tristate 'Codepage 861 (Icelandic)'              CONFIG_NLS_CODEPAGE_861
  tristate 'Codepage 862 (Hebrew)'                 CONFIG_NLS_CODEPAGE_862
  tristate 'Codepage 863 (Canadian French)'        CONFIG_NLS_CODEPAGE_863
  tristate 'Codepage 864 (Arabic)'                 CONFIG_NLS_CODEPAGE_864
  tristate 'Codepage 865 (Norwegian, Danish)'      CONFIG_NLS_CODEPAGE_865
  tristate 'Codepage 866 (Cyrillic/Russian)'       CONFIG_NLS_CODEPAGE_866
  tristate 'Codepage 869 (Greek)'                  CONFIG_NLS_CODEPAGE_869
  tristate 'Simplified Chinese charset (CP936, GB2312)' CONFIG_NLS_CODEPAGE_936
  tristate 'Traditional Chinese charset (Big5)'    CONFIG_NLS_CODEPAGE_950
  tristate 'Japanese charsets (Shift-JIS, EUC-JP)' CONFIG_NLS_CODEPAGE_932
  tristate 'Korean charset (CP949, EUC-KR)'        CONFIG_NLS_CODEPAGE_949
  tristate 'Thai charset (CP874, TIS-620)'         CONFIG_NLS_CODEPAGE_874
  tristate 'Hebrew charsets (ISO-8859-8, CP1255)'  CONFIG_NLS_ISO8859_8
  tristate 'Windows CP1250 (Slavic/Central European Languages)' CONFIG_NLS_CODEPAGE_1250
  tristate 'Windows CP1251 (Bulgarian, Belarusian)' CONFIG_NLS_CODEPAGE_1251
  tristate 'NLS ISO 8859-1  (Latin 1; Western European Languages)' CONFIG_NLS_ISO8859_1
  tristate 'NLS ISO 8859-2  (Latin 2; Slavic/Central European Languages)' CONFIG_NLS_ISO8859_2
  tristate 'NLS ISO 8859-3  (Latin 3; Esperanto, Galician, Maltese, Turkish)' CONFIG_NLS_ISO8859_3
  tristate 'NLS ISO 8859-4  (Latin 4; old Baltic charset)' CONFIG_NLS_ISO8859_4
  tristate 'NLS ISO 8859-5  (Cyrillic)'             CONFIG_NLS_ISO8859_5
  tristate 'NLS ISO 8859-6  (Arabic)'               CONFIG_NLS_ISO8859_6
  tristate 'NLS ISO 8859-7  (Modern Greek)'         CONFIG_NLS_ISO8859_7
  tristate 'NLS ISO 8859-9  (Latin 5; Turkish)'     CONFIG_NLS_ISO8859_9
  tristate 'NLS ISO 8859-13 (Latin 7; Baltic)'      CONFIG_NLS_ISO8859_13
  tristate 'NLS ISO 8859-14 (Latin 8; Celtic)'      CONFIG_NLS_ISO8859_14
  tristate 'NLS ISO 8859-15 (Latin 9; Western European Languages with Euro)' CONFIG_NLS_ISO8859_15
  tristate 'NLS KOI8-R (Russian)'                   CONFIG_NLS_KOI8_R
  tristate 'NLS KOI8-U/RU (Ukrainian, Belarusian)' CONFIG_NLS_KOI8_U
  tristate 'NLS UTF8'                               CONFIG_NLS_UTF8
  endmenu
  
nls_koi8-ru.c 
nls_sjis.c  
nls_tis-620.c

*/

static const struct charset_entry
{
    const char *charset_name;
    const char *nls;
    unsigned int        codepage;
} charset_names[] =
{
    { "BIG5",      "big5", 950 },	/* 2.4 ? 2.6 */
    { "CP1250",    "cp1250", 1250 },	/* // 2.4 */
    { "CP1251",    "cp1251", 1251 }, /* // 2.4 */
    { "CP1252",    "cp1252", 1252 }, /* // */
    { "CP1253",    "cp1253", 1253 }, /* // */
    { "CP1254",    "cp1254", 1254 }, /* // */
    { "CP1255",    "cp1255", 1255 }, /* // 2.4 */
    { "CP1256",    "cp1256", 1256 }, /* // */
    { "CP1257",    "cp1257", 1257 }, /* // */
    { "CP1258",    "cp1258", 1258 }, /* // */
    { "CP932",     "cp932", 932 },	/* // 2.4 */
    { "CP936",     "cp936", 936 },	/* // 2.4 */
    { "CP949",     "cp949", 949 },	/* // 2.4 */
    { "CP950",     "cp950", 950 },	/* // 2.4 */
    { "EUCJP",     "euc-jp", 20932 }, /* // 2.4 euc-jp */
    { "GB2312",    "gb2312", 936 },	/* // 2.4 ? 2.6 */
    { "IBM037",    "cp037", 37 },
    { "IBM1026",   "cp1026", 1026 },
    { "IBM424",    "cp424", 424 },
    { "IBM437",    "cp437", 437 }, /* // 2.4 cp Codepage 437 (United States, Canada) */
    { "IBM500",    "cp500", 500 },
/* Unknown for iconv */
/*    { "IBM737",    "cp737", 737 },*/ /* // 2.4/6 */
/*    { "IBM775",    "cp775", 775 },*/ /* // 2.4/6 */

    { "IBM850",    "cp850", 850 }, /* // 2.4 cp */
    { "IBM852",    "cp852", 852 }, /* // 2.4 cp */
    { "IBM855",    "cp855", 855 }, /* // 2.4 cp */
    { "IBM857",    "cp857", 857 }, /* // 2.4 cp */
    { "IBM860",    "cp860", 860 }, /* // 2.4 cp */
    { "IBM861",    "cp861", 861 }, /* // 2.4 cp */
    { "IBM862",    "cp862", 862 }, /* // 2.4 cp */
    { "IBM863",    "cp863", 863 }, /* // 2.4 cp */
    { "IBM864",    "cp864", 864 }, /* // 2.4 cp */
    { "IBM865",    "cp865", 865 }, /* // 2.4 cp */
    { "IBM866",    "cp866", 866 }, /* // 2.4 cp */
    { "IBM869",    "cp869", 869 }, /* // 2.4 cp */
    { "IBM874",    "cp874", 874 }, /* // 2.4 cp */
    { "IBM875",    "cp875", 875 },
    { "ISO88591",  "iso8859-1", 28591 }, /* // 2.4 с чертой */
    { "ISO885910", "iso8859-10", 28600 }, /* // 2.4 с чертой */
    { "ISO885913", "iso8859-13", 28603 }, /* // 2.4 с чертой */
    { "ISO885914", "iso8859-14", 28604 }, /* // 2.4 с чертой */
    { "ISO885915", "iso8859-15", 28605 }, /* // 2.4 с чертой */
    { "ISO885916", "iso8859-16", 28606 },
    { "ISO88592",  "iso8859-2", 28592 }, /* // 2.4 с чертой */
    { "ISO88593",  "iso8859-3", 28593 }, /* // 2.4 с чертой */
    { "ISO88594",  "iso8859-4", 28594 }, /* // 2.4 с чертой */
    { "ISO88595",  "iso8859-5", 28595 }, /* // 2.4 с чертой */
    { "ISO88596",  "iso8859-6", 28596 }, /* // 2.4 с чертой */
    { "ISO88597",  "iso8859-7", 28597 }, /* // 2.4 с чертой */
    { "ISO88598",  "iso8859-8", 28598 }, /* // 2.4 с чертой */
    { "ISO88599",  "iso8859-9", 28599 }, /* // 2.4 с чертой */
    { "KOI8R",     "koi8-r", 20866 }, /* // 2.4 с чертой */
    { "KOI8U",     "koi8-u", 21866 }, /* // 2.4 с чертой */
    { "MAC",     "MAC", 10000 },
    { "MACCYRILLIC", "MAC-CYRILLIC", 10007 },
    { "MACUKRAINIAN", "MACUKRAINIAN", 10017 },
    { "UTF8",      "utf8", 65001 }
};
