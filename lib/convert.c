/*
    lib/convert.c
	
	safe convert between encoding (with losses)

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2002, 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: convert.c,v 1.9 2005/02/26 15:23:06 lav Exp $

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

/* Open iconv table */
static iconv_t _natspec_iconv_open(const char *tocode, const char *fromcode)
{
	if ( tocode == NULL || !strlen(tocode))
		tocode = natspec_get_charset();
	if ( fromcode == NULL || !strlen(fromcode))
		fromcode = natspec_get_charset();

	return iconv_open(tocode, fromcode);
}

static int ucs2_cmp( const void *ucs2, const void *entry )
{
	const struct u7_struct *s = (const struct u7_struct*)entry;
	const unsigned short u = *((const unsigned short*)ucs2);
	return (u - s->x);
}

/* Get transliterated UCS2 character */
static const char *get_7bit (unsigned short ucs2)
{
	struct u7_struct *entry;
	entry = bsearch( &ucs2, unicode_7b,
	  	sizeof(unicode_7b) / sizeof(unicode_7b[0]),
	    sizeof(unicode_7b[0]), ucs2_cmp );
	if (entry == NULL || entry->s == NULL)
		return "_";
	DEBUG (printf("ucs2:%x, %s, entry:%x\n",ucs2, entry->s, entry));
	return entry->s;
}

/* Returns converts input string from encoding to encoding
 * Source: from my old patch for XMMS (2002 year)
 */
char *natspec_convert_with_translit(const char *in_str,
	const char *tocode, const char *fromcode)
{
	size_t result;
	unsigned short tmp;
	iconv_t frt, ucs2;
	size_t lena = strlen(in_str)*6; /* FIXME see E2BIG for errno */
	size_t lenb = strlen(in_str);
	size_t lentmp;
	char *ansa = (char*)malloc(lena+1);
	char *ansbptr = (char*)in_str;
	char *ansaptr = ansa, *tmpptr;

	frt = _natspec_iconv_open(tocode, fromcode);
	ucs2 = _natspec_iconv_open("UCS2", fromcode);
	if (frt == (iconv_t) (-1) || ucs2 == (iconv_t) (-1))
	{
		free (ansa);
		return NULL;
	}
	for (;;)
	{
		DEBUG (printf("%s:%d %d [%c]\n",ansbptr,lenb,lenb,*ansbptr));
		result = iconv(frt, &ansbptr, &lenb, &ansaptr, &lena);
		if (result != (size_t) -1)
			break;
		if ( errno != EILSEQ )
			break;
		/* Replace invalid input character. See code of links, sim, iconv */
		lentmp = 2;
		tmpptr = (char*) &tmp;
		DEBUG (printf("%d, %s:%d\n",ucs2,ansbptr,lenb));
		result = iconv(ucs2, &ansbptr, &lenb, &tmpptr, &lentmp);
		if ((result == (size_t) -1 && errno == E2BIG) || result != (size_t) -1)
		{
			DEBUG (printf("br\n"));
			const char *t = get_7bit(tmp);
			strcpy(ansaptr, t);
			ansaptr += strlen(t);
			lena -= 2;
		}
		else
		{
			DEBUG (printf("%d '%c'\n",errno, *ansbptr));
			DEBUG (perror(""));
			*ansaptr++ = '_';
			lena--;
			ansbptr++;
			lenb--;
		}
	}
	iconv_close(frt);
	/*ansa[strlen(in_str) - lenb] = '\0';*/
	*ansaptr = '\0';
	ansaptr = strdup(ansa);
	free(ansa);
	return ansaptr;
}


char *natspec_convert(const char *in_str,
	const char *tocode, const char *fromcode)
{
	size_t result;
	iconv_t frt;
	size_t lena = strlen(in_str)*6; /* FIXME see E2BIG for errno */
	size_t lenb = strlen(in_str);
	char *ansa = (char*)malloc(lena+1);
	char *ansbptr = (char*)in_str;
	char *ansaptr = ansa;
	char *ret = NULL;

	frt = _natspec_iconv_open(tocode, fromcode);
	if (frt != (iconv_t) (-1))
	{
		result = iconv(frt, &ansbptr, &lenb, &ansaptr, &lena);
		if (result != (size_t) -1)
		{
			*ansaptr = '\0';
			ret = strdup(ansa);
		}
		iconv_close(frt);
	}
	free(ansa);
	return ret;
}

