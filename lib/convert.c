/*
    lib/convert.c
	
	sure convert between encoding (with transliteration if needed)

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2002, 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: convert.c,v 1.20 2008/02/14 22:36:22 vitlav Exp $

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation, version 2.1
    of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA.
*/

#include <iconv.h>
#include <errno.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "natspec_internal.h"
#include "unicode/uni_7b.h"

#define UCS4_SIZE 4

/* UCS-4 descriptors (named ucs2 for history) for transliteration,
   one per natspec_iconv_open */
struct ucs2_entry
{
	iconv_t it, ucs2;
	struct ucs2_entry *next;
};
static struct ucs2_entry *ucs2list;
static pthread_mutex_t ucs2list_lock = PTHREAD_MUTEX_INITIALIZER;

/*! Internal: returns UCS2 descriptor for cd or 0 */
static iconv_t find_ucs2(iconv_t cd)
{
	struct ucs2_entry *e;
	iconv_t ucs2 = 0;
	pthread_mutex_lock(&ucs2list_lock);
	for (e = ucs2list; e; e = e->next)
		if (e->it == cd) {
			ucs2 = e->ucs2;
			break;
		}
	pthread_mutex_unlock(&ucs2list_lock);
	return ucs2;
}

/*! Opens iconv table or returns -1 */
iconv_t natspec_iconv_open(const char *tocode, const char *fromcode)
{
	struct ucs2_entry *e;
	if ( _n_isempty(tocode) )
		tocode = natspec_get_charset();
	if ( _n_isempty(fromcode) )
		fromcode = natspec_get_charset();
	e = malloc(sizeof(*e));
	if (!e)
		return (iconv_t)-1;
	e->it = iconv_open(tocode, fromcode);
	if (e->it == (iconv_t)-1) {
		free(e);
		return (iconv_t)-1;
	}
	e->ucs2 = iconv_open("UCS-4BE", fromcode);
	if (e->ucs2 == (iconv_t)-1) {
		iconv_close(e->it);
		free(e);
		return (iconv_t)-1;
	}
	pthread_mutex_lock(&ucs2list_lock);
	e->next = ucs2list;
	ucs2list = e;
	pthread_mutex_unlock(&ucs2list_lock);
	return e->it;
}

/*! Closes descriptor opened with natspec_iconv_open */
int natspec_iconv_close(iconv_t cd)
{
	struct ucs2_entry **p, *e = NULL;
	pthread_mutex_lock(&ucs2list_lock);
	for (p = &ucs2list; *p; p = &(*p)->next)
		if ((*p)->it == cd) {
			e = *p;
			*p = e->next;
			break;
		}
	pthread_mutex_unlock(&ucs2list_lock);
	if (e) {
		iconv_close(e->ucs2);
		free(e);
	}
	return iconv_close(cd);
}

/* use __const due typedef int (*__compar_fn_t) (__const void *, __const void *); */
static int ucs2_cmp( __const void *ucs2, __const void *entry )
{
	const struct u7_struct *s = (const struct u7_struct*)entry;
	const unsigned short *u = (const unsigned short*)ucs2;
	return *u - s->x;
}

/*! Get transliterated UCS2 character or '_' if it is impossible */
static const char *get_7bit (unsigned short ucs2)
{
	const struct u7_struct *entry;
	entry = _n_bsearch( &ucs2, unicode_7b, ucs2_cmp );
	if (entry == NULL || entry->s == NULL)
		return "_";
	DEBUG (printf("ucs2:%x, %s, entry:%x\n",ucs2, entry->s, entry));
	return entry->s;
}

/*! convert as iconv but can transliterate (returns -1 if error)
 * Source: from my old patch for XMMS (2002 year) and sim code */
size_t natspec_iconv(iconv_t cd, char **inbuf, size_t *inbytesleft,
	char **outbuf, size_t *outbytesleft, int transliterate)
{
	iconv_t ucs2 = 0;
	size_t result;
	for(;;)
	{
		DEBUG (printf("%d (ucs2: %d), %s:%d\n",cd, ucs2, *inbuf, *inbytesleft));
		result = iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
		if (result != (size_t) -1)
			return result; /* done converting successfully */
		if ( errno != EILSEQ )
			return result;
		if (!transliterate)
			return result;
		/* find corresponding UCS2 descriptor */
		if (!ucs2)
			ucs2 = find_ucs2(cd);
		if (!ucs2)
			return result;
		else
		{
			/* Replace invalid input character. See code of links, sim, iconv, catdoc */
			size_t tmpresult;
			unsigned char tmp[UCS4_SIZE];
			size_t lentmp = UCS4_SIZE;
			char *tmpptr = (char*) tmp;
			char *saveinbuf = *inbuf;
			size_t saveinbytesleft = *inbytesleft;
			DEBUG (printf("replace: %d, %s:%d\n",ucs2, *inbuf, *inbytesleft));
			tmpresult = iconv(ucs2, inbuf, inbytesleft, &tmpptr, &lentmp);
			if (tmpptr == (char*) tmp + UCS4_SIZE &&
				(tmpresult != (size_t) -1 || errno == E2BIG))
			{
				unsigned long ucs4 = ((unsigned long)tmp[0] << 24) |
					((unsigned long)tmp[1] << 16) | (tmp[2] << 8) | tmp[3];
				/* only BMP characters are in the transliteration table */
				const char *t = ucs4 > 0xFFFF ? "_" : get_7bit(ucs4);
				size_t tlen = strlen(t);
				if (tlen > *outbytesleft)
				{
					/* unread the character, caller have to give more space */
					*inbuf = saveinbuf;
					*inbytesleft = saveinbytesleft;
					iconv(ucs2, NULL, NULL, NULL, NULL);
					errno = E2BIG;
					return (size_t) -1;
				}
				memcpy(*outbuf, t, tlen);
				(*outbuf) += tlen;
				(*outbytesleft) -= tlen;
				DEBUG (printf("br\n"));
			}
			else
			{	/* if iconv could not help, skip one byte from input string */
				/* DEBUG (printf("%d '%c'\n",errno, *ansbptr));
				DEBUG (perror("")); */
				*inbuf = saveinbuf;
				*inbytesleft = saveinbytesleft;
				iconv(ucs2, NULL, NULL, NULL, NULL);
				if (*outbytesleft < 1)
				{
					errno = E2BIG;
					return (size_t) -1;
				}
				**outbuf = '_';
				(*outbuf)++;
				(*inbytesleft)--;
				(*inbuf)++;
				(*outbytesleft)--;
				result += 1;
			}
		}
	}
}

/*! Returns converts input string from encoding to encoding
 * TODO: Locale depends transliterating
 */
char *natspec_convert(const char *in_str,
	const char *tocode, const char *fromcode, int transliterate)
{
	iconv_t frt;
	size_t lenb, lena, size;
	char *ansa, *ansaptr, *ansbptr = (char*)in_str;

	if (!in_str)
		return NULL;
	lenb = strlen(in_str);

	frt = natspec_iconv_open(tocode, fromcode);
	if (frt == (iconv_t) -1)
	{
		char buf[200];
		snprintf(buf,199,"Broken encoding: '%s' (to) or '%s' (from) or UCS-4. May be you forget setlocale in main or gconv-modules is missed?\n",
			tocode, fromcode);
		perror(buf);
		/* return dup of original string if converting is failed */
		ansaptr = strdup(in_str);
		return ansaptr;
	}

	/* one byte is reserved for the terminating '\0' */
	size = lenb + 16;
	ansa = malloc(size + 1);
	ansaptr = ansa;
	lena = size;
	while (ansa)
	{
		size_t res;
		int flush = !lenb;
		if (!flush)
			res = natspec_iconv(frt, &ansbptr, &lenb, &ansaptr, &lena, transliterate);
		else /* flush shift state of the output */
			res = iconv(frt, NULL, NULL, &ansaptr, &lena);
		if (res == (size_t) -1 && errno == E2BIG)
		{
			size_t done = ansaptr - ansa;
			char *tmp = realloc(ansa, size * 2 + 1);
			if (!tmp)
			{
				free(ansa);
				ansa = NULL;
				break;
			}
			ansa = tmp;
			ansaptr = ansa + done;
			lena += size;
			size *= 2;
			continue;
		}
		/* other errors are ignored: return converted part */
		if (res == (size_t) -1 || flush)
			break;
	}
	natspec_iconv_close(frt);
	if (ansa)
		*ansaptr = '\0';
	return ansa;
}


/* Obsoleted */
char *natspec_convert_with_translit(const char *in_str,
	const char *tocode, const char *fromcode)
{
	return natspec_convert(in_str, tocode, fromcode, 1);
}

