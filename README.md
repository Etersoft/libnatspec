LIBNATSPEC [![Build Status](https://travis-ci.org/vitlav/libnatspec.svg?branch=master)](https://travis-ci.org/vitlav/libnatspec) [![Coverity Scan Build Status](https://scan.coverity.com/projects/vitlav-libnatspec/badge.svg)](https://scan.coverity.com/projects/vitlav-libnatspec)
=====

    LIBNATSPEC
	The Nation Specifity Library
	Vitaly Lipatov <lav@etersoft.ru>

About libnatspec
----------------

libnatspec is a collection of function for requiest various
charsets and locales for host system and for other system.
The libnatspec allows applications to be written that are
locale and system independent. Libnatspec is tries to use
user locale information if possible.

It contains table with relations between locale name and encoding
of other system.

It tested in various OS (with GCC installed):

 + ALT Linux 2.4 Master (x86)
 + Debian GNU/Linux 3.0 (Alpha)
 - Solaris SunOS 5.9 (Sparc/x86) (missed libiconv during linking)
 ? Slackware (x86)
 + Mandrake (x86)
 + RedHat (x86)
 ? ASPLinux (x86)
 - FreeBSD 4.10 (x86) (missed popt)
It is not tested in other Unix-like system yet.

It has been tested with various Linux programs/servers:
 + mount
 + mkisofs / isoinfo
 + smbmount
 + submount (subfs)
 - smbd/nmbd/swat/smbclient
 + zip/unzip

This distribution contains library, header files and binding
to other languages as python (--with-python option for configure)

Please if you discover some bug or misfunction of the natspec library
on your system, email me about it.

Installation
------------

Installation instructions are found in INSTALL.
For RPM-based system you can use libnatspec.spec as an example.

More Information
----------------

libnatspec may be freely distibuted and modified in accordance with
the GNU Lesser General Public License. Examples in the libnatspec
distribution can fall under different licenses;
see the example/LICENSE file and individual files for details.

The person for contact: Vitaly Lipatov <lav@etersoft.ru>

Examples of use
---------------

### Convert files from other system to our one
```
// Get the filename encoding in your system
const char *fileenc = natspec_get_filename_encoding("");

// Get the charset in other system WIN
const char *win_cs = natspec_get_charset_by_locale(NATSPEC_WINCS,"");

// Do conversion from foreign to local filename encoding
const char * unix_filename = natspec_convert(win_filename,win_cs, fileenc);
```

### Using in .c files

```
#ifdef HAVE_NATSPEC
#include <natspec.h>
#endif
...
#ifdef HAVE_NATSPEC
	some with natspec using
#endif
```

### Using in configure.in files

1. Add line with AM_PATH_NATSPEC in configure.in
it makes NATSPEC_LIBS, NATSPEC_CFLAGS variables for Makefiles
and defines HAVE_NATSPEC in config.h
2. Use option --with-natspec for configure

07.05.2005, 24.10.2015
