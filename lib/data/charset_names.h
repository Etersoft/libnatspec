/* This code contains charset to codepage map originally from
   WINE project (file dlls/kernel/locale.c)
	LGPL license
   $Id: charset_names.h,v 1.10 2007/07/30 09:01:50 vitlav Exp $
 */

/* 
TODO:
 - some gcc option for pack a static string?

Description was from Linux kernel fs/nls/Config.in
Some strange nls from Linux kernel:
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
    { "BIG5",      "big5",        950 },  /* Traditional Chinese charset (Big5) */
    { "CP1125",    "cp1125",      1125 }, /* Codepage 1125 (Ukrainian DOS codepage) */
    { "CP1250",    "cp1250",      1250 }, /* Windows CP1250 (Slavic/Central European Languages) */
    { "CP1251",    "cp1251",      1251 }, /* Windows CP1251 (Bulgarian, Belarusian) */
    { "CP1252",    "cp1252",      1252 }, /* European Languages */
    { "CP1253",    "cp1253",      1253 }, /* Greek */
    { "CP1254",    "cp1254",      1254 }, /* Turkish */
    { "CP1255",    "cp1255",      1255 }, /* Hebrew charsets (ISO-8859-8, CP1255) */
    { "CP1256",    "cp1256",      1256 }, /* // */
    { "CP1257",    "cp1257",      1257 }, /* Estonian, Latvian, Lithuanian */
    { "CP1258",    "cp1258",      1258 }, /* // */
    { "CP932",     "cp932",        932 }, /* Japanese charsets (Shift-JIS, EUC-JP) */
    { "CP936",     "cp936",        936 }, /* Simplified Chinese charset (CP936, GB2312) */
    { "CP949",     "cp949",        949 }, /* Korean charset (CP949, EUC-KR) */
    { "CP950",     "cp950",        950 }, /* // 2.4 */
    { "EUCJP",     "euc-jp",     20932 }, /* // 2.4 euc-jp */
    { "GB2312",    "gb2312",       936 }, /* // 2.4 ? 2.6 */
    { "IBM037",    "cp037",         37 },
    { "IBM1026",   "cp1026",      1026 },
    { "IBM424",    "cp424",        424 },
    { "IBM437",    "cp437",        437 }, /* // 2.4 cp Codepage 437 (United States, Canada) */
    { "IBM500",    "cp500",        500 },
/* Unknown for iconv */
/*    { "IBM737",    "cp737", 737 },*/ /* // 2.4/6 Codepage 737 (Greek)*/
/*    { "IBM775",    "cp775", 775 },*/ /* // 2.4/6 Codepage 775 (Baltic Rim)*/

    { "IBM850",    "cp850",        850 }, /* // 2.4 cp Codepage 850 (Europe)*/
    { "IBM852",    "cp852",        852 }, /* Codepage 852 (Central/Eastern Europe) */
    { "IBM855",    "cp855",        855 }, /* Codepage 855 (Cyrillic) */
    { "IBM857",    "cp857",        857 }, /* Codepage 857 (Turkish) */
    { "IBM860",    "cp860",        860 }, /* Codepage 860 (Portuguese) */
    { "IBM861",    "cp861",        861 }, /* Codepage 861 (Icelandic) */
    { "IBM862",    "cp862",        862 }, /* Codepage 862 (Hebrew) */
    { "IBM863",    "cp863",        863 }, /* Codepage 863 (Canadian French) */
    { "IBM864",    "cp864",        864 }, /* Codepage 864 (Arabic) */
    { "IBM865",    "cp865",        865 }, /* Codepage 865 (Norwegian, Danish) */
    { "IBM866",    "cp866",        866 }, /* Codepage 866 (Cyrillic/Russian) */
    { "IBM869",    "cp869",        869 }, /* Codepage 869 (Greek) */
    { "IBM874",    "cp874",        874 }, /* Thai charset (CP874, TIS-620) */
    { "IBM875",    "cp875",        875 },
    { "ISO88591",  "iso8859-1",  28591 }, /* NLS ISO 8859-1  (Latin 1; Western European Languages) */
    { "ISO885910", "iso8859-10", 28600 }, /* // 2.4 с чертой */
    { "ISO885913", "iso8859-13", 28603 }, /* NLS ISO 8859-13 (Latin 7; Baltic) */
    { "ISO885914", "iso8859-14", 28604 }, /* NLS ISO 8859-14 (Latin 8; Celtic) */
    { "ISO885915", "iso8859-15", 28605 }, /* NLS ISO 8859-15 (Latin 9; Western European Languages with Euro) */
    { "ISO885916", "iso8859-16", 28606 },
    { "ISO88592",  "iso8859-2",  28592 }, /* NLS ISO 8859-2  (Latin 2; Slavic/Central European Languages) */
    { "ISO88593",  "iso8859-3",  28593 }, /* NLS ISO 8859-3  (Latin 3; Esperanto, Galician, Maltese, Turkish) */
    { "ISO88594",  "iso8859-4",  28594 }, /* NLS ISO 8859-4  (Latin 4; old Baltic charset) */
    { "ISO88595",  "iso8859-5",  28595 }, /* NLS ISO 8859-5  (Cyrillic) */
    { "ISO88596",  "iso8859-6",  28596 }, /* NLS ISO 8859-6  (Arabic) */
    { "ISO88597",  "iso8859-7",  28597 }, /* NLS ISO 8859-7  (Modern Greek) */
    { "ISO88598",  "iso8859-8",  28598 }, /* Hebrew charsets (ISO-8859-8, CP1255) */
    { "ISO88599",  "iso8859-9",  28599 }, /* NLS ISO 8859-9  (Latin 5; Turkish) */
    { "KOI8R",     "koi8-r",     20866 }, /* KOI8-R (Russian) */
    { "KOI8U",     "koi8-u",     21866 }, /* KOI8-U/RU (Ukrainian, Belarusian) */
    { "MAC",     "MAC",          10000 },
    { "MACCYRILLIC", "MAC-CYRILLIC", 10007 },
    { "MACUKRAINIAN", "MACUKRAINIAN", 10017 },
    { "UTF8",      "utf8",       65001 }
};
