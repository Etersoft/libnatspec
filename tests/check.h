/*
 * Public domain
 *
 * Minimal test harness: checks do not stop the test,
 * check_done() returns exit code for automake test driver
 */

#ifndef __NATSPEC_CHECK_H
#define __NATSPEC_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* automake exit code for skipped test */
#define CHECK_SKIP 77

static int check_failed, check_total;

#define CHECK(cond) do { \
		check_total++; \
		if (!(cond)) { \
			check_failed++; \
			fprintf(stderr, "%s:%d: FAIL: %s\n", __FILE__, __LINE__, #cond); \
		} \
	} while (0)

#define CHECK_INT(got, expected) do { \
		long _g = (long)(got), _e = (long)(expected); \
		check_total++; \
		if (_g != _e) { \
			check_failed++; \
			fprintf(stderr, "%s:%d: FAIL: %s: got %ld, expected %ld\n", \
				__FILE__, __LINE__, #got, _g, _e); \
		} \
	} while (0)

/* compares strings, NULL is equal only to NULL */
#define CHECK_STR(got, expected) do { \
		const char *_g = (got), *_e = (expected); \
		check_total++; \
		if ((_g == NULL || _e == NULL) ? _g != _e : strcmp(_g, _e) != 0) { \
			check_failed++; \
			fprintf(stderr, "%s:%d: FAIL: %s: got '%s', expected '%s'\n", \
				__FILE__, __LINE__, #got, _g ? _g : "(null)", _e ? _e : "(null)"); \
		} \
	} while (0)

/* compares malloc allocated string and frees it */
#define CHECK_STR_FREE(got, expected) do { \
		char *_s = (got); \
		CHECK_STR(_s, expected); \
		free(_s); \
	} while (0)

static int check_done(void)
{
	printf("%d checks, %d failed\n", check_total, check_failed);
	return check_failed ? 1 : 0;
}

#endif
