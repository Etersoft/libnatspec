/*
 * Public domain
 *
 * libFuzzer harness: the natspec utility with random arguments.
 * src/natspec.c is built with -Dmain=natspec_main -Dexit=natspec_fuzz_exit.
 * Input: '\0' separated strings: LC_ALL value, then the arguments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <fcntl.h>

int natspec_main(int argc, const char **argv);

/* global option variables of src/natspec.c */
extern char *charset_type, *locale, *transliterate;
extern int version, verbose, fsenc, fcodepage, nls, flag_help, info;
extern int utf8, country_id, get_system_locale, get_user_locale;

static jmp_buf exit_jmp;

void natspec_fuzz_exit(int code)
{
	(void)code;
	longjmp(exit_jmp, 1);
}

#define MAXARGS 10

int LLVMFuzzerTestOneInput(const unsigned char *data, size_t size)
{
	static int quiet;
	const char *argv[MAXARGS + 1];
	char *buf, *p, *end;
	int argc = 0;

	if (!quiet)
	{
		/* drop the output of the utility */
		int fd = open("/dev/null", O_WRONLY);
		dup2(fd, 1);
		quiet = 1;
	}

	buf = malloc(size + 1);
	memcpy(buf, data, size);
	buf[size] = '\0';
	end = buf + size;

	/* the first string is LC_ALL */
	setenv("LC_ALL", buf, 1);
	unsetenv("G_FILENAME_ENCODING");
	argv[argc++] = "natspec";
	for (p = buf + strlen(buf) + 1; p < end && argc < MAXARGS; p += strlen(p) + 1)
		argv[argc++] = p;
	argv[argc] = NULL;

	charset_type = locale = transliterate = NULL;
	version = verbose = fsenc = fcodepage = nls = flag_help = info = 0;
	utf8 = country_id = get_system_locale = get_user_locale = 0;

	if (!setjmp(exit_jmp))
		natspec_main(argc, argv);
	fflush(stdout);

	free(buf);
	return 0;
}
