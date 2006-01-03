Name: libnatspec
Version: 0.2.3
Release: alt2.1

Summary: Library for national and language-specific issues

License: LGPL
Group: System/Libraries
Url: http://www.etersoft.ru/natspec

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: http://www.etersoft.ru/download/%name-%version.tar.bz2

# manually removed: gcc-g77 xorg-x11-libs gcc-c++ libstdc++-devel libg2c-devel 
# Automatically added by buildreq on Fri Jul 22 2005
BuildRequires: doxygen libpopt-devel python-base python-modules-encodings

%description
Library for national and language-specific issues.
This library provides userful functions for
mount, submount, mkisofs, multimedia players.
This library try to help resolve charset hell (encoding problem)
in a various programs depends on locale and messages.
See detailed description at %url.

%package devel
Summary: Development package of library for national and language-specific issues
Group: Development/C
Requires: %name = %version-%release

%description devel
The %name-devel package contains the necessary include files
for developing applications with %name
This library try to help resolve charset hell (encoding problem)
in a various programs depends on locale and messages.

%package devel-examples
Summary: Examples of %name using
Group: Development/Documentation

%description devel-examples
The %name-devel package contains examples of patches
for developing applications with %name

%package -n python-module-natspec
Summary: Python binding
Group: Development/Python
Requires: %name = %version-%release

%description -n python-module-natspec
Python binding for natspec

%prep
%setup -q

%build
%configure
%make_build

%install
%makeinstall

# FIXME: I don't know how to install in /lib
# move to /lib
cd %buildroot
mkdir -p %_lib && mv usr/%_lib/%{name}.* %_lib

%post
%post_ldconfig

%postun
%postun_ldconfig

%files
%doc AUTHORS COPYING README ChangeLog NEWS TODO README-ru.html
/%_lib/*.so.*
%_bindir/*
#/etc/profile.d/*

%files devel
%doc docs/html
%_includedir/*
/%_lib/%name.so
%_pkgconfigdir/*
%_datadir/aclocal/*

%files devel-examples
%doc examples profile

#%files -n python-module-natspec
#%python_sitelibdir/natspec.py
#%python_sitelibdir/_natspec.so


%changelog
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

