/*
    lib/convert.c
	
	safe convert between encoding (with losses)

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2002, 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: convert.c,v 1.3 2005/02/23 15:03:27 lav Exp $

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

/* Returns converts input string from encoding to encoding
 * Source: from my old patch for XMMS (2002 year)
 */
char *natspec_convert(const char *in_str,
	const char *tocode, const char *fromcode)
{
	size_t result;
	iconv_t frt;
	size_t lena = strlen(in_str)*2; /* FIXME see E2BIG for errno */
	size_t lenb = strlen(in_str);
	char *ansa = (char*)malloc(lena+1);
	char *ansbptr = (char*)in_str;
	char *ansaptr = ansa;
	frt = iconv_open(tocode, fromcode);
	if (frt == (iconv_t) -1)
	{
	    DEBUG (perror ("iconv_open()"));
	    return NULL;
	}

	for (;;)
	{
		result = iconv(frt, &ansbptr, &lenb, &ansaptr, &lena);
		if (result != (size_t) -1)
			break;
		if ( errno == EILSEQ )
		{
			/* Replace invalid input character FIXME: see iconv util code
			*ansaptr++ = *ansbptr; */
			*ansaptr++ = '_';
			lena--;
			ansbptr++;
			lenb--;
			continue;
		}
		DEBUG (perror ("iconv()"));
		return NULL;
	}
	ansa[strlen(in_str) - lenb] = 0;

	if (iconv_close(frt) != 0)
	    DEBUG (perror ("iconv_close()"));
	return ansa;
}
