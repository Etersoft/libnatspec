/*
    lib/natspec.h
	
	natspec library internal header file

    NATSPEC library
    Copyright (c) 2005 Etersoft
    Copyright (c) 2005 Vitaly Lipatov <lav@etersoft.ru>

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
#include "../config.h"
#endif

#include "natspec.h"

#ifdef HAVE_NL_LANGINFO
# define HAVE_LANGINFO_CODESET
#endif

#define DEBUG(n)

char *repack_locale(const char *locale);

struct charsetrel_entry
{
    const char *locale;
    unsigned int lcid;
    const char *unix_cs;
    const char *win_cs;
    const char *dos_cs;
    const char *mac_cs;
};


#endif
