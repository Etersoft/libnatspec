Name: libnatspec
Version: 0.4.0
Release: alt1

Summary: Library for national and language-specific issues

License: LGPL-2.1-only
Group: System/Libraries
Url: https://github.com/Etersoft/libnatspec

Packager: Vitaly Lipatov <lav@altlinux.ru>

# Source-git: https://github.com/Etersoft/libnatspec
Source: %name-%version.tar

# Automatically added by buildreq on Fri Jul 22 2005
BuildRequires: libpopt-devel
# due aclocaldir
BuildRequires: rpm-build-compat
# for tests: iconv modules for KOI8-R, CP1251, MACINTOSH and others
BuildRequires: glibc-gconv-modules

%description
libnatspec knows which charsets are used for a locale on this system
and on Windows, DOS and Mac, and converts strings between them with
transliteration of missed characters.

It helps programs to get file names and mount options right:
iocharset/codepage for FAT, NTFS, ISO 9660, UDF and SMB/CIFS,
file names in archives made on Windows, and so on.

This package contains the shared library and the natspec utility.

%package devel
Summary: Development package of library for national and language-specific issues
Group: Development/C
Requires: %name = %version-%release

%description devel
The %name-devel package contains the header file, pkg-config file
and autoconf macro (AM_PATH_NATSPEC) for developing applications
with %name.

%package devel-examples
Summary: Examples of patches adding %name support to programs
Group: Development/Documentation
BuildArch: noarch

%description devel-examples
Examples of patches which add %name support to unzip, zip, p7zip,
mount, mkisofs, samba and other programs, and profile scripts
setting G_FILENAME_ENCODING.

%package -n python-module-natspec
Summary: Python binding
Group: Development/Python
Requires: %name = %version-%release

%description -n python-module-natspec
Python binding for natspec

%prep
%setup

%build
%autoreconf
%configure
%make_build

%check
%make_build check

%install
%makeinstall_std

# FIXME: I don't know how to install in /lib
# move to /lib
mkdir -p %buildroot/%_lib
mv %buildroot%_libdir/%{name}.* %buildroot/%_lib

%files
%doc AUTHORS README ChangeLog NEWS TODO README-ru.html
/%_lib/*.so.*
%_bindir/*
%_man1dir/*
#/etc/profile.d/*

%files devel
#doc docs/html
%_includedir/*
/%_lib/%name.so
%_pkgconfigdir/*
%_aclocaldir/*

%files devel-examples
%doc examples profile

#%files -n python-module-natspec
#%python_sitelibdir/natspec.py
#%python_sitelibdir/_natspec.so


%changelog
* Sat Oct 17 2026 Vitaly Lipatov <lav@altlinux.ru> 0.4.0-alt1
- security: fix stack buffer overflow in natspec_iconv()/natspec_convert()
  with transliteration (since 0.1.0)
- security: natspec_convert(): use heap buffer instead of alloca()
  (stack exhaustion on big input)
- natspec_humble_charset(): use malloc() instead of alloca()
- natspec_iconv_open(): mutex protected list instead of the global table
  of 5 descriptors (thread safety, leaks after failed open)
- do not crash on empty charset, return NULL from natspec_convert(NULL)
- read system locale from /etc/locale.conf, then /etc/sysconfig/i18n
- parse i18n file by lines with shell-like syntax (SUPPORTED_LANG= is not LANG=)
- use charset from locale name if it is not in the table
  (be_BY.CP1251, de_DE.UTF-8@euro, zh_CN.GB2312)
- find C, POSIX and C.UTF-8 locales, fix sorting of the locale table
- recognize charset aliases (windows-1251, CP866, macintosh)
- use G_FILENAME_ENCODING for the current locale only, take the first
  charset of the comma separated list
- data: kk_KZ uses CP1251/IBM866 as in Windows, Russian charsets for
  Cyrillic languages of Russia (ce, cv, kv, mdf, mhr, os)
- data: keep manual corrections in charset_overrides.txt (applied by gen_data.sh)
- transliteration: fix duplicate and wrong entries, NO-BREAK SPACE gives space,
  one '_' for a character outside BMP
- python binding: regenerate with SWIG 4, works with python3
- add test suite (make check), run it in %check
- build: fix configure with dash and -Werror (popt was not found),
  make distcheck and VPATH builds, link with libiconv if needed,
  drop AC_FUNC_MALLOC (cross build) and __DATE__ (reproducible build)
- natspec: clarify --transl help (ALT bug 47189)
- License: LGPL-2.1-only (SPDX), rewrite package descriptions
- profile scripts are not executable
- natspec: fix use after free of --transl argument with popt 1.19

* Mon May 12 2025 Vitaly Lipatov <lav@altlinux.ru> 0.3.5-alt1
- update data generator code
- configure.ac: fix AC_ARG_WITH using
- update charset relation data (with gen_data.sh)

* Fri May 09 2025 Kirill Izmestev <felixz@altlinux.org> 0.3.4-alt1
- Add Ossetian locale support.

* Fri Oct 04 2024 Kirill Izmestev <felixz@altlinux.org> 0.3.3-alt2
- Fixed Tatar locale name as glibc.

* Sat Aug 12 2023 Vitaly Lipatov <lav@altlinux.ru> 0.3.3-alt1
- enrich_fs_options: fix warning about buffer overflow
- netspec: use natspec_convert() for transliteration via -a
- doc: natspec -a transliterate and print only one arg

* Wed Jul 26 2023 Vitaly Lipatov <lav@altlinux.ru> 0.3.2-alt1
- update doc files

* Sat Jul 22 2023 Kirill Izmestev <felixz@altlinux.ru> 0.3.1-alt3
- Add Mari locale support
- Add Komi locale support

* Sat Oct 24 2015 Vitaly Lipatov <lav@altlinux.ru> 0.3.1-alt2
- update version and add comment about automate it
- add p7zip patch from https://github.com/buzztaiki/pkgbuild-p7zip-natspec/
- fix URL and path to source

* Sat Oct 24 2015 Vitaly Lipatov <lav@altlinux.ru> 0.3.1-alt1
- add travis support
- fix some build issues, thanks to Travis CI and Coverity
- update version and cleanup doxygen file
- natspec: fix error with compiling without popt
- fix printf size_t issues
- get_charset: fix memory leak detected by Coverity

* Sun Aug 16 2015 Vitaly Lipatov <lav@altlinux.ru> 0.3.0-alt1
- netspec cli: always print with \n
- README-ru recode to utf8

* Sat Aug 15 2015 Vitaly Lipatov <lav@altlinux.ru> 0.2.8-alt1
- natspec: add missed -l (--locale) option

* Wed Feb 04 2015 Vitaly Lipatov <lav@altlinux.ru> 0.2.7-alt1
- remove example sources, fix authors in patches, add patch for unzip-6.0
- natspec command: small rewrite output

* Tue Apr 23 2013 Repocop Q. A. Robot <repocop@altlinux.org> 0.2.6-alt2.qa3
- NMU (by repocop). See http://www.altlinux.org/Tools/Repocop
- applied repocop fixes:
  * arch-dep-package-consists-of-usr-share for libnatspec-devel-examples

* Fri Oct 05 2012 Eugeny A. Rostovtsev (REAL) <real at altlinux.org> 0.2.6-alt2.qa2
- Rebuilt for debuginfo (ALT #27806)

* Thu Nov 25 2010 Igor Vlasenko <viy@altlinux.ru> 0.2.6-alt2.qa1
- rebuild using girar-nmu to require/provide setversion 
  by request of mithraen@

* Sun Mar 07 2010 Vitaly Lipatov <lav@altlinux.ru> 0.2.6-alt2
- set in natspec.m4 require only 0.2.4 version of libnatspec for build

* Sun Jan 10 2010 Vitaly Lipatov <lav@altlinux.ru> 0.2.6-alt1
- cleanup spec
- regenerate supported locales list
- check kk_KZ.UTF-8 locale support (fix alt bug #21957)
- do not use toupper_l/tolower_l (fix alt bug #21431)

* Sat Sep 05 2009 Vitaly Lipatov <lav@altlinux.ru> 0.2.5-alt1
- update spec
- convert to Git, build with gear

* Fri Feb 15 2008 Vitaly Lipatov <lav@altlinux.ru> 0.2.4-alt1
- natspec_convert returns original string if failed (fix bug #14301)
- small bugfixes, clean code

* Wed Jan 02 2008 Vitaly Lipatov <lav@altlinux.ru> 0.2.3-alt1
- small fixes
- fix build (publish tarball with all autogenerated files)

* Fri Mar 24 2006 Vitaly Lipatov <lav@altlinux.ru> 0.2.3-alt0.2beta
- fix bug with null pointer (thanks to rider@)

* Mon Mar 13 2006 Vitaly Lipatov <lav@altlinux.ru> 0.2.3-alt0.1beta
- new version (with natspec_iconv), see NEWS
- remove COPYING

* Wed Aug 03 2005 Vitaly Lipatov <lav@altlinux.ru> 0.2.2-alt2.1
- really fix unexpected macros

* Sat Jul 30 2005 Vitaly Lipatov <lav@altlinux.ru> 0.2.2-alt2
- remove gcc-c++ requires
- fix spec (thanks to php-coder@)
- fix packages' groups
- fix bug #7495 (unexpected macros)

* Fri Jul 22 2005 Vitaly Lipatov <lav@altlinux.ru> 0.2.2-alt1
- small fixes only (see NEWS)
- add html API doc in -devel
- update buildreq

* Sun Apr 10 2005 Vitaly Lipatov <lav@altlinux.ru> 0.2.1-alt1
- new release, small fixes

* Thu Mar 17 2005 Vitaly Lipatov <lav@altlinux.ru> 0.2.0-alt1
- new stable release

* Thu Mar 03 2005 Vitaly Lipatov <lav@altlinux.ru> 0.1.9-alt1
- 0.2pre release

* Sun Feb 27 2005 Vitaly Lipatov <lav@altlinux.ru> 0.1.0-alt1
- first stable release

* Sun Feb 27 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.9-alt1
- new version (see NEWS)

* Thu Feb 24 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.4-alt1
- new version (add descriptions, cleanup headers)

* Wed Feb 23 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.3-alt1
- new version (fix bug with unix charset)

* Mon Feb 21 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.2-alt1
- new version

* Sun Feb 20 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.1-alt1
- first public release (for ALT Linux Sisyphus)

