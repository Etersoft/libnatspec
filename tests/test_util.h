/*
 * Public domain
 *
 * Helpers for tests
 */

#ifndef __NATSPEC_TEST_UTIL_H
#define __NATSPEC_TEST_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* writes content to a new temporary file and returns its malloc allocated name */
static char *test_write_file(const char *content)
{
	const char *dir = getenv("TMPDIR");
	char *name;
	int fd;
	FILE *f;
	if (!dir || !*dir)
		dir = "/tmp";
	name = malloc(strlen(dir) + 32);
	if (!name)
		abort();
	sprintf(name, "%s/natspec-test-XXXXXX", dir);
	fd = mkstemp(name);
	if (fd < 0 || !(f = fdopen(fd, "w")))
	{
		perror(name);
		exit(99); /* hard error for automake */
	}
	fputs(content, f);
	fclose(f);
	return name;
}

static void test_remove_file(char *name)
{
	unlink(name);
	free(name);
}

#endif
