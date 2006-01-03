/*
    lib/convert.c
	
	sure convert between encoding (with transliteration if needed)

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2002, 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: convert.c,v 1.15 2006/01/03 00:54:45 vitlav Exp $

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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
    USA.
*/

#include <iconv.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "natspec_internal.h"
#include "unicode/uni_7b.h"

#define UCS2_SIZE 2

/* FIXME: static ucs2 :( Need associate with iconv_t */
static iconv_t ucs2;

/*! Open iconv table */
iconv_t natspec_iconv_open(const char *tocode, const char *fromcode)
{
	if ( _n_isempty(tocode) )
		tocode = natspec_get_charset();
	if ( _n_isempty(fromcode) )
		fromcode = natspec_get_charset();

	ucs2 = iconv_open("UCS2", fromcode);
	return iconv_open(tocode, fromcode);
}

static int ucs2_cmp( const void *ucs2, const void *entry )
{
	const struct u7_struct *s = (const struct u7_struct*)entry;
	const unsigned short *u = (const unsigned short*)ucs2;
	return *u - s->x;
}

/*! Get transliterated UCS2 character or '_' if it is impossible */
static const char *get_7bit (unsigned short ucs2)
{
	struct u7_struct *entry;
	entry = _n_bsearch( &ucs2, unicode_7b, ucs2_cmp );
	if (entry == NULL || entry->s == NULL)
		return "_";
	DEBUG (printf("ucs2:%x, %s, entry:%x\n",ucs2, entry->s, entry));
	return entry->s;
}

/*! convert as iconv but can transliterate */
size_t natspec_iconv(iconv_t cd, char **inbuf, size_t *inbytesleft,
	char **outbuf, size_t *outbytesleft, int transliterate)
{
	size_t result;
	for(;;)
	{
		//DEBUG (printf("%s\n",*inbuf));
		DEBUG (printf("%d (ucs2: %d), %s:%d\n",cd, ucs2, *inbuf, *inbytesleft));
		result = iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
		if (result != (size_t) -1)
			return result; /* done converting successfully */
		if ( errno != EILSEQ )
			return result;
		if (!transliterate || !ucs2)
			return result;
		else
		{
			/* Replace invalid input character. See code of links, sim, iconv, catdoc */
			unsigned short tmp;
			size_t lentmp = UCS2_SIZE;
			char *tmpptr = (char*) &tmp;
			DEBUG (printf("replace: %d, %s:%d\n",ucs2, *inbuf, *inbytesleft));
			result = iconv(ucs2, inbuf, inbytesleft, &tmpptr, &lentmp);
			if ((result == (size_t) -1 && errno == E2BIG) || result != (size_t) -1)
			{
				const char *t = get_7bit(tmp);
				strcpy(*outbuf, t);
				(*outbuf) += strlen(t);
				(*outbytesleft) -= UCS2_SIZE;
				DEBUG (printf("br\n"));
			}
			else
			{	/* if iconv could not help, skip one byte from input string */
				/* DEBUG (printf("%d '%c'\n",errno, *ansbptr));
				DEBUG (perror("")); */
				**outbuf = '_';
				(*outbuf)++;
				(*inbytesleft)--;
				(*inbuf)++;
				(*outbytesleft)--;
			}
		}
	}
}

/*! Returns converts input string from encoding to encoding
 * Source: from my old patch for XMMS (2002 year) and sim code
 * TODO: Locale depends transliterating
 */
char *natspec_convert_with_translit(const char *in_str,
	const char *tocode, const char *fromcode)
{
	size_t result;
	iconv_t frt, ucs2;
	size_t lena = strlen(in_str)*6; /* FIXME see E2BIG for errno */
	size_t lenb = strlen(in_str);
	char *ansa = (char*)alloca(lena+1);
	char *ansbptr = (char*)in_str;
	char *ansaptr = ansa;

	frt = natspec_iconv_open(tocode, fromcode);
	ucs2 = natspec_iconv_open("UCS2", fromcode);
	if (frt == (iconv_t) -1 || ucs2 == (iconv_t) -1)
	{
		char buf[100];
		snprintf(buf,99,"Broken encoding: '%s' or '%s' (%d) or UCS2 (%d). May be you forget setlocale in main?\n",
			tocode, fromcode, (int)frt, (int)ucs2);
		perror(buf);
		return NULL;
	}
	for (;;)
	{
		DEBUG (printf("%s:%d %d [%c]\n",ansbptr,lenb,lenb,*ansbptr));
		result = iconv(frt, &ansbptr, &lenb, &ansaptr, &lena);
		if (result != (size_t) -1)
			break; /* done converting successfully */
		if ( errno != EILSEQ )
			break;
		/* Replace invalid input character. See code of links, sim, iconv, catdoc */
		{
		unsigned short tmp;
		size_t lentmp = UCS2_SIZE;
		char *tmpptr = (char*) &tmp;
		DEBUG (printf("%d, %s:%d\n",ucs2,ansbptr,lenb));
		result = iconv(ucs2, &ansbptr, &lenb, &tmpptr, &lentmp);
		if ((result == (size_t) -1 && errno == E2BIG) || result != (size_t) -1)
		{
			const char *t = get_7bit(tmp);
			strcpy(ansaptr, t);
			ansaptr += strlen(t);
			lena -= UCS2_SIZE;
			DEBUG (printf("br\n"));
		}
		else
		{	/* if iconv could not help, skip one byte from input string */
			DEBUG (printf("%d '%c'\n",errno, *ansbptr));
			DEBUG (perror(""));
			*ansaptr++ = '_';
			lena--;
			ansbptr++;
			lenb--;
		}
		}
	}
	iconv_close(frt);
	*ansaptr = '\0';
	ansaptr = strdup(ansa);
	return ansaptr;
}


char *natspec_convert(const char *in_str,
	const char *tocode, const char *fromcode)
{
	iconv_t frt;
	size_t lena = strlen(in_str)*6; /* FIXME see E2BIG for errno */
	size_t lenb = strlen(in_str);
	char *ansa = (char*)alloca(lena+1);
	char *ansbptr = (char*)in_str;
	char *ansaptr = ansa;
	char *ret = NULL;

	frt = natspec_iconv_open(tocode, fromcode);
	if (frt != (iconv_t) (-1))
	{
		size_t result = iconv(frt, &ansbptr, &lenb, &ansaptr, &lena);
		if (result != (size_t) -1)
		{
			*ansaptr = '\0';
			ret = strdup(ansa);
		}
		iconv_close(frt);
	}
	return ret;
}

