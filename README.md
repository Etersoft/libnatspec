LIBNATSPEC [![Build Status](https://travis-ci.org/vitlav/libnatspec.svg?branch=master)](https://travis-ci.org/vitlav/libnatspec) [![Coverity Scan Build Status](https://scan.coverity.com/projects/vitlav-libnatspec/badge.svg)](https://scan.coverity.com/projects/vitlav-libnatspec)
=====

    LIBNATSPEC
	The Nation Specifity Library
	Vitaly Lipatov <lav@etersoft.ru>

About libnatspec
----------------

libnatspec is designed to smooth out national peculiarities when using software. Its primary objectives are:
- Addressing encoding issues in most popular scenarios.
- Providing various auxiliary tools that facilitate software localization.

Linux users often encounter encoding problems, both with file content and file names. These issues arise when:
- Mounting various storage devices like floppy disks, CDs, flash drives, and external hard drives.
- Mounting network resources via SAMBA.
- Burning discs or creating file systems using commands like mkisofs, growisofs, and software like k3b, xcdroast.
- Using the file manager 'mc' with its built-in text recoding feature.

Moreover, there's a vast array of software where encoding specifications would be beneficial
but are currently lacking, such as FTP servers and clients, file systems used for OS installation,
and many multimedia players.

libnatspec provides a solution to these challenges by offering a centralized approach
to handle encoding-related tasks, enhancing portability and freeing individual software projects
from implementing ad-hoc solutions.

The library defines essential concepts like:
- Local file system encoding (filename encoding).
- User locale.
- System locale.
- Charset of the user locale.
- Charset and codepage of other operating systems (WIN, DOS, MAC) for a given locale.

It also offers an API for these concepts and additional functions that allow:
- Augmenting mount parameters with encodings based on the file system type.
- Converting strings from one encoding to another, with transliteration if necessary.

It has been tested in various OS (with GCC installed):

 + ALT Linux 2.4 Master (x86)
 + Debian GNU/Linux 3.0 (Alpha)
 - Solaris SunOS 5.9 (Sparc/x86) (missed libiconv during linking)
 ? Slackware (x86)
 + Mandrake (x86)
 + RedHat (x86)
 ? ASPLinux (x86)
 - FreeBSD 4.10 (x86) (missed popt)
It is not tested in other Unix-like system yet.

It also has been tested with various Linux/server software:
 + mount
 + mkisofs / isoinfo
 + smbmount
 + submount (subfs)
 - smbd/nmbd/swat/smbclient
 + zip/unzip

This distribution contains library, header files and binding
to other languages as python (--with-python option for configure)

If you discover some bug or misfunction of the natspec library
on your system, please email me about it.

Installation
------------

Installation instructions are placed in the INSTALL file.
For RPM-based systems you can use libnatspec.spec as a reference.

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
