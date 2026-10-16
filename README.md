LIBNATSPEC — The Nation Specifity Library
=========================================

[![Packaging status](https://repology.org/badge/latest-versions/libnatspec.svg)](https://repology.org/project/libnatspec/versions)

libnatspec smooths out national peculiarities of a system: it knows which
character sets are used for a locale here and on other systems (Windows,
DOS, Mac), and converts strings between them.

Encoding problems with file names and contents appear when you:

 * mount FAT/NTFS/ISO 9660/UDF media: flash drives, external disks, CDs;
 * mount SMB/CIFS network shares;
 * unpack archives made on Windows (zip, 7z) or burn discs (mkisofs);
 * exchange files with FTP servers and clients, multimedia players, etc.

Instead of every program guessing encodings on its own, libnatspec gives
one answer for all of them, based on the user locale.

Concepts
--------

The library defines and provides API for:

 * user locale and system locale;
 * charset of a locale;
 * local file system encoding (filename encoding);
 * charset and code page of other systems (Windows, DOS, Mac) for a locale.

And helpers to:

 * add `iocharset=`, `codepage=` and `nls=` options to mount options
   according to the file system type;
 * convert strings between encodings, with transliteration of characters
   which the target encoding does not have (`Ж` → `ZH`, `∂` → `\partial`).

Examples for the `ru_RU.UTF-8` locale:

| Question                               | Answer           |
|----------------------------------------|------------------|
| charset of the locale                  | UTF8             |
| Windows / DOS / Mac charset            | CP1251 / IBM866 / MACCYRILLIC |
| mount options for `vfat`               | `iocharset=utf8,codepage=866` |
| mount options for `ntfs`               | `nls=utf8`       |

API
---

All functions are declared in `natspec.h`. Functions returning `char *`
return a newly allocated string which must be freed with `free()`;
`const char *` results point to static data.

| Function | Description |
|----------|-------------|
| `natspec_get_current_locale()` | locale from `LC_ALL`, `LC_CTYPE`, `LANG`, or the system locale |
| `natspec_get_system_locale()` | `LANG` from the system i18n file (`/etc/sysconfig/i18n` by default) |
| `natspec_get_charset()` | charset of the current locale (after `setlocale`) |
| `natspec_get_charset_by_locale(type, locale)` | charset of `NATSPEC_UNIXCS`, `NATSPEC_WINCS`, `NATSPEC_DOSCS` or `NATSPEC_MACCS` for a locale (`""` is the current locale, `NULL` is the system one) |
| `natspec_locale_is_utf8(locale)` | non-zero for a UTF-8 locale |
| `natspec_get_filename_encoding(locale)` | filename encoding in kernel nls form (`G_FILENAME_ENCODING` is honored for the current locale) |
| `natspec_get_enriched_fs_options(fs, options)` | mount options with encoding options added |
| `natspec_get_codepage_by_charset(cs)`, `natspec_get_nls_by_charset(cs)` | code page number and kernel nls name of a charset |
| `natspec_convert(str, tocode, fromcode, transliterate)` | convert a string (`NULL` or `""` charset is the current one) |
| `natspec_iconv_open()`, `natspec_iconv()`, `natspec_iconv_close()` | iconv-like API with transliteration |

Charset names are accepted in any common form: `KOI8-R`, `koi8r`,
`windows-1251`, `CP866`.

### Convert file names from Windows to the local encoding

```c
#include <locale.h>
#include <natspec.h>

setlocale(LC_ALL, "");

/* filename encoding of this system */
const char *fileenc = natspec_get_filename_encoding("");
/* charset of Windows for the current locale */
const char *win_cs = natspec_get_charset_by_locale(NATSPEC_WINCS, "");

/* the last argument enables transliteration */
char *unix_filename = natspec_convert(win_filename, fileenc, win_cs, 1);
...
free(unix_filename);
```

### Add encoding options to mount options

```c
char *options = natspec_get_enriched_fs_options("vfat", "ro,noexec");
/* "ro,noexec,iocharset=utf8,codepage=866" in ru_RU.UTF-8 */
...
free(options);
```

### Use in your project

Use `pkg-config --cflags --libs libnatspec`, or the autoconf macro from
`natspec.m4`:

1. Add `AM_PATH_NATSPEC` to `configure.ac`. It sets `NATSPEC_LIBS` and
   `NATSPEC_CFLAGS` for Makefiles and defines `HAVE_NATSPEC` in `config.h`.
2. Run configure with `--with-natspec`.
3. Make natspec support optional in the code:

```c
#ifdef HAVE_NATSPEC
#include <natspec.h>
#endif
```

Patches which add natspec support to unzip, zip, p7zip, mount, mkisofs,
samba, pmount and other programs are in the `examples/` directory.

The natspec utility
-------------------

```
natspec --info              # overall locale and encoding information
natspec --locale            # current user locale
natspec --system-locale     # system locale
natspec --fsenc             # filename encoding
natspec --charset win       # charset of other system: unix, win, dos, mac
natspec --codepage          # DOS code page
natspec --utf8              # exit code 0 if the current locale is UTF-8
natspec --transl ENC STRING # convert STRING from ENC to the current encoding
                            # with transliteration
```

See `natspec --help` and `man natspec`.

Python binding
--------------

Configure with `--with-python` to build the `natspec` module (SWIG):

```python
import natspec
natspec.natspec_convert("Привет", "ASCII", "UTF-8", 1)   # 'Privet'
```

Building
--------

Requirements: a C compiler, autoconf, automake, libtool, iconv.
Optional: popt (for the `natspec` utility), Python and SWIG (Python
binding), Doxygen (API documentation).

```
autoreconf -fiv
./configure [--with-python] [--with-doxygen] [--with-i18n-file=FILE]
make
make check
make install
```

`--with-i18n-file` sets the file with the system wide `LANG` variable
(`/etc/sysconfig/i18n` by default).

For RPM-based systems see `libnatspec.spec`.

Data
----

The table of locales and their Windows/DOS/Mac charsets
(`lib/data/get_charset_data.h`) is generated from Wine locale data by
`lib/data/gen_data.sh`. Manual corrections are kept in
`lib/data/charset_overrides.txt` with the source of every value.
If a charset for your locale is wrong, please report it with a reference
(for example, to the Microsoft NLS API Reference).

Packages
--------

[![Packaging status](https://repology.org/badge/vertical-allrepos/libnatspec.svg)](https://repology.org/project/libnatspec/versions)

natspec-enabled builds of unzip, zip, p7zip and 7zip are available
in ALT Linux and AUR.

License and contacts
--------------------

libnatspec may be freely distributed and modified in accordance with
the GNU Lesser General Public License, version 2.1. Examples in the
distribution can fall under different licenses; see `examples/LICENSE`
and individual files for details.

Source code: https://github.com/Etersoft/libnatspec

Please report bugs there or to Vitaly Lipatov <lav@etersoft.ru>.
