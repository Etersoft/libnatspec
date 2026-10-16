LIBNATSPEC [![Coverity Scan Build Status](https://scan.coverity.com/projects/vitlav-libnatspec/badge.svg)](https://scan.coverity.com/projects/vitlav-libnatspec)
=====

    LIBNATSPEC
	The Nation Specifity Library
	Vitaly Lipatov <lav@etersoft.ru>

About libnatspec
----------------

libnatspec is a collection of functions for requesting various
charsets and locales for the host system and for other systems
(Windows, DOS, Mac). It allows applications to be written that are
locale and system independent. Libnatspec tries to use user locale
information if possible.

It contains a table with relations between locale names and encodings
of other systems, and helpers to:

 * detect the current user and system locale and its charset;
 * get the filename encoding;
 * get the Windows/DOS/Mac charset and codepage for a locale;
 * add proper `iocharset=`/`codepage=` options for mounting filesystems;
 * convert strings between encodings (iconv wrappers with transliteration).

This distribution contains the library, header files, the `natspec`
command line utility and a binding to Python (`--with-python` option
for configure).

Please if you discover some bug or misfunction of the natspec library
on your system, email me about it.

Building
--------

Requirements: a C compiler, autotools, libtool, iconv.
Optional: popt (for the `natspec` utility), Python and SWIG (Python binding),
Doxygen (API documentation).

```
autoreconf -fiv
./configure [--with-python] [--with-doxygen] [--with-debug] [--with-i18n-file=FILE]
make
make install
```

`--with-i18n-file` sets the file with the system wide LANG variable
(`/etc/sysconfig/i18n` by default).

Run the tests with `make check`.

For RPM-based systems you can use libnatspec.spec as an example.
More installation instructions are found in INSTALL.

The natspec utility
-------------------

```
natspec --info              # overall encoding/charset info for your system
natspec --locale            # current user locale
natspec --system-locale     # system locale
natspec --fsenc             # filesystem (filename) encoding
natspec --charset win       # charset for other system: unix, win, dos, mac
natspec --codepage          # codepage
natspec --utf8              # exit code 0 if the locale is UTF-8
natspec --transl ENC STRING # transliterate STRING from ENC to the current encoding
```

See `natspec --help` and `man natspec` for details.

Examples of use
---------------

### Convert files from other system to our one
```
// Get the filename encoding in your system
const char *fileenc = natspec_get_filename_encoding("");

// Get the charset in other system WIN
const char *win_cs = natspec_get_charset_by_locale(NATSPEC_WINCS, "");

// Do conversion from foreign to local filename encoding
// (the last argument enables transliteration of inconvenient symbols)
char *unix_filename = natspec_convert(win_filename, fileenc, win_cs, 0);
...
free(unix_filename);
```

Functions returning `char *` return a newly allocated string which must be
freed with `free()`. Do not forget to call `setlocale(LC_ALL, "")` first.

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

### Using in configure.ac files

1. Add line with AM_PATH_NATSPEC in configure.ac
it makes NATSPEC_LIBS, NATSPEC_CFLAGS variables for Makefiles
and defines HAVE_NATSPEC in config.h
2. Use option --with-natspec for configure

Alternatively use pkg-config: `pkg-config --cflags --libs libnatspec`.

Patches adding natspec support to various programs (mount, mkisofs,
unzip, zip, samba, pmount and others) can be found in the `examples/`
directory.

Tested systems
--------------

Historically (2005) it was tested in various OS (with GCC installed):

 + ALT Linux 2.4 Master (x86)
 + Debian GNU/Linux 3.0 (Alpha)
 - Solaris SunOS 5.9 (Sparc/x86) (missed libiconv during linking)
 ? Slackware (x86)
 + Mandrake (x86)
 + RedHat (x86)
 ? ASPLinux (x86)
 - FreeBSD 4.10 (x86) (missed popt)

and with various Linux programs/servers:

 + mount
 + mkisofs / isoinfo
 + smbmount
 + submount (subfs)
 - smbd/nmbd/swat/smbclient
 + zip/unzip

License
-------

libnatspec may be freely distributed and modified in accordance with
the GNU Lesser General Public License. Examples in the libnatspec
distribution can fall under different licenses;
see the examples/LICENSE file and individual files for details.

The person for contact: Vitaly Lipatov <lav@etersoft.ru>

07.05.2005, 24.10.2015, 16.10.2026
