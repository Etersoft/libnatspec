/*
 * Public domain
 *
 * Tests for descriptors of natspec_iconv_open:
 * - failed open does not leave resources busy
 * - many descriptors can be open at the same time
 * - concurrent use from threads
 */

#include <pthread.h>

#include "check.h"
#include "natspec.h"

/* failed opens must not exhaust internal resources */
static void test_failed_opens(void)
{
	int i;
	for (i = 0; i < 20; i++)
		CHECK(natspec_iconv_open("NONEXISTENT", "UTF-8") == (iconv_t)-1);
	CHECK_STR_FREE(natspec_convert("\xD0\x96", "ASCII", "UTF-8", 1), "ZH");
}

/* many descriptors open at the same time */
static void test_many_open(void)
{
	iconv_t cd[20];
	int i;
	for (i = 0; i < 20; i++)
	{
		cd[i] = natspec_iconv_open("ASCII", "UTF-8");
		CHECK(cd[i] != (iconv_t)-1);
	}
	for (i = 0; i < 20; i++)
	{
		char in[] = "\xD0\x96", out[8];
		char *inp = in, *outp = out;
		size_t il = 2, ol = sizeof(out) - 1;
		if (cd[i] == (iconv_t)-1)
			continue;
		natspec_iconv(cd[i], &inp, &il, &outp, &ol, 1);
		*outp = '\0';
		CHECK_STR(out, "ZH");
	}
	for (i = 0; i < 20; i++)
		if (cd[i] != (iconv_t)-1)
			natspec_iconv_close(cd[i]);
}

#define THREADS 8
#define ITERATIONS 2000

static void *convert_thread(void *arg)
{
	int i, *failed = arg;
	for (i = 0; i < ITERATIONS; i++)
	{
		char *out = natspec_convert("ab\xD0\x96\xD0\x96zz", "ASCII", "UTF-8", 1);
		if (!out || strcmp(out, "abZHZHzz"))
			(*failed)++;
		free(out);
	}
	return NULL;
}

static void test_threads(void)
{
	pthread_t th[THREADS];
	int failed[THREADS];
	int i;
	for (i = 0; i < THREADS; i++)
	{
		failed[i] = 0;
		pthread_create(&th[i], NULL, convert_thread, &failed[i]);
	}
	for (i = 0; i < THREADS; i++)
	{
		pthread_join(th[i], NULL);
		CHECK_INT(failed[i], 0);
	}
}

int main(void)
{
	test_failed_opens();
	test_many_open();
	test_threads();
	return check_done();
}
