Name: libnatspec
Version: 0.0.5
Release: alt1

Summary: Nation & languages specifity issues library

License: LGPL
Group: Development/C
Url: http://www.etersoft.ru/natspec

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: http://www.etersoft.ru/download/%name-%version.tar.bz2

# manually removed: gcc-g77 xorg-x11-libs
# Automatically added by buildreq on Wed Feb 23 2005
BuildRequires: gcc-c++ hostinfo libpopt-devel libstdc++-devel python-base python-modules-encodings

%description
Nation & languages specifity issues library
This library provides userful functions for
	mount, submount, mkisofs, multimedia players
See detailed description at %Url

%package devel
Summary: NATSPEC - resolves charset hell in system
Group: Development/C
Requires: %name = %version-%release

%description devel
The %name-devel package contains the necessary include files
for developing applications with %name
This library try to resolve charset hell (encoding problem)
in various program depends on locale.

%package devel-examples
Summary: Examples of %name using
Group: Development/C

%description devel-examples
The %name-devel package contains examples of patches
for developing applications with %name

%package -n python-module-natspec
Summary: Python binding
Group: Development/C
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
mkdir -p %_lib
mv usr/%_lib/lib* %_lib

%post -p %post_ldconfig

%postun -p %postun_ldconfig

%files
%doc AUTHORS COPYING README ChangeLog NEWS TODO README-ru.html
/%_lib/*.so.*
%_bindir/*
#/etc/profile.d/%name*

%files devel
%_includedir/*
/%_lib/%name.so
%_libdir/pkgconfig/*

%files devel-examples
%doc examples

#%files -n python-module-natspec
#%python_sitelibdir/natspec.py
#%python_sitelibdir/_natspec.so


%changelog
* Thu Feb 24 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.4-alt1
- new version (add descriptions, cleanup headers)

* Wed Feb 23 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.3-alt1
- new version (fix bug with unix charset)

* Mon Feb 21 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.2-alt1
- new version

* Sun Feb 20 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.1-alt1
- first public release (for ALT Linux Sisyphus)

