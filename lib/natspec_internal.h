/*
    lib/natspec.h
	
	natspec library internal header file

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

    $Id: natspec_internal.h,v 1.12 2005/03/09 20:18:02 lav Exp $

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


#ifndef __NATSPEC_INTERNAL_H
#define __NATSPEC_INTERNAL_H

#ifdef HAVE_CONFIG_H
#	include "../config.h"
#else
#	error Something is broken
#endif

#include "natspec.h"

#ifdef HAVE_NL_LANGINFO
#	define HAVE_LANGINFO_CODESET
#else
	#error "Some error"
#endif

#ifdef __USE_GNU
#	define _n_toupper(n) toupper_l(n,"C")
#	define _n_tolower(n) tolower_l(n,"C")
#else
#	define _n_toupper(n) toupper(n)
#	define _n_tolower(n) tolower(n)
#	warning "We can't use locale independent lower/upper without GNU"
#endif
#define _n_isalnum(n) isalnum(n)

/* Add n in next line for debug messaging */
#define DEBUG(n)

/* Extracts charset (if exist) from locale, removes punctiation
 * signs from it, pack locale again
 * and return malloc allocated locale string
 */
char *_natspec_repack_locale(const char *locale);

/* Struct of charset_relation table */
struct charsetrel_entry
{
    const char *locale;	/* Unix locale name (from glibc) */
    unsigned int lcid;	/* Windows locale id (from WINE) */
			/* for this locale: (can be NULL) */
    const char *unix_cs;  /* unix charset */
    const char *win_cs;   /* windows charset */
    const char *dos_cs;   /* dos charset */
    const char *mac_cs;   /* mac charset */
};


#endif
