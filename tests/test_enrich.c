/*
 * Public domain
 *
 * Tests for natspec_get_enriched_fs_options
 * (mount options according to the current locale)
 */

#include "check.h"
#include "natspec.h"

static void check_enrich(const char *fs, const char *options, const char *expected)
{
	char *got = natspec_get_enriched_fs_options(fs, options);
	check_total++;
	if ((got == NULL || expected == NULL) ? got != expected : strcmp(got, expected) != 0)
	{
		check_failed++;
		fprintf(stderr, "FAIL: enrich(%s, '%s') with LC_ALL=%s: got '%s', expected '%s'\n",
			fs ? fs : "(null)", options ? options : "(null)", getenv("LC_ALL"),
			got ? got : "(null)", expected ? expected : "(null)");
	}
	free(got);
}

static void test_koi8r(void)
{
	setenv("LC_ALL", "ru_RU.KOI8-R", 1);
	check_enrich("vfat", NULL, "iocharset=koi8-r,codepage=866");
	check_enrich("vfat", "", "iocharset=koi8-r,codepage=866");
	check_enrich("vfat", "defaults", "defaults,iocharset=koi8-r,codepage=866");
	check_enrich("vfat", "ro,", "ro,iocharset=koi8-r,codepage=866");
	check_enrich("fat", "ro", "ro,iocharset=koi8-r,codepage=866");
	check_enrich("msdos", "ro", "ro,iocharset=koi8-r,codepage=866");
	check_enrich("iso9660", "ro", "ro,iocharset=koi8-r");
	check_enrich("udf", "ro", "ro,iocharset=koi8-r");
	check_enrich("jfs", "ro", "ro,iocharset=koi8-r");
	check_enrich("ntfs", "ro", "ro,nls=koi8-r");
	check_enrich("smbfs", "ro", "ro,iocharset=koi8-r,codepage=cp866");
	check_enrich("cifs", "ro", "ro,iocharset=koi8-r,codepage=cp866");
	/* unknown fs */
	check_enrich("ext4", "ro", "ro");
	check_enrich("reiserfs", "defaults", "defaults");
	check_enrich(NULL, "ro", "ro");
	check_enrich(NULL, NULL, "");
}

static void test_utf8(void)
{
	setenv("LC_ALL", "ru_RU.UTF-8", 1);
	check_enrich("vfat", "ro", "ro,iocharset=utf8,codepage=866");
	check_enrich("iso9660", "", "iocharset=utf8");
	check_enrich("ntfs", NULL, "nls=utf8");
	setenv("LC_ALL", "en_US.UTF-8", 1);
	check_enrich("vfat", "ro", "ro,iocharset=utf8,codepage=437");
	setenv("LC_ALL", "de_DE@euro", 1);
	check_enrich("vfat", "ro", "ro,iocharset=iso8859-15,codepage=850");
}

static void test_already_set(void)
{
	setenv("LC_ALL", "ru_RU.KOI8-R", 1);
	/* encoding options are already set: do not change */
	check_enrich("vfat", "iocharset=koi8-r", "iocharset=koi8-r");
	check_enrich("vfat", "ro,codepage=850", "ro,codepage=850");
	check_enrich("ntfs", "nls=utf8", "nls=utf8");
	check_enrich("vfat", "utf8,iocharset=utf8", "utf8,iocharset=utf8");
}

int main(void)
{
	unsetenv("LC_CTYPE");
	unsetenv("LANG");
	unsetenv("G_FILENAME_ENCODING");
	test_koi8r();
	test_utf8();
	test_already_set();
	return check_done();
}
