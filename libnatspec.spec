Name: libnatspec
Version: 0.0.3
Release: alt1

Summary: Nation & languages specifity issues library

License: LGPL
Group: Development/C
Url: http://www.etersoft.ru/natspec

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: http://www.etersoft.ru/download/%name-%version.tar.bz2

# manually removed: gcc-g77  xorg-x11-libs
# Automatically added by buildreq on Sun Feb 20 2005
BuildRequires: gcc-c++ hostinfo libpopt-devel libstdc++-devel

%description
Nation & languages specifity issues library
This library provides userful functions for
	mount, submount, mkisofs, multimedia players

%package devel
Summary: The files needed for %name application development
Group: Development/C
Requires: %name = %version-%release

%description devel
The %name-devel package contains the necessary include files
for developing applications with %name

%package devel-examples
Summary: Examples of %name using
Group: Development/C

%description devel-examples
The %name-devel package contains examples of patches
for developing applications with %name

%package python-module-natspec
Summary: Python binding
Group: Development/C
Requires: %name = %version-%release

%description python-module-natspec
Python binding for natspec

%prep
%setup -q

%build
%configure
%make_build
#cd python
#CFLAGS="$RPM_OPT_FLAGS" %__python setup.py build


%install
%makeinstall
#%__python setup.py install --root $RPM_BUILD_ROOT

# move to /lib
cd %buildroot
mkdir -p %_lib
mv usr/%_lib/lib* %_lib

%post -p %post_ldconfig

%postun -p %postun_ldconfig

%files
%doc AUTHORS COPYING README ChangeLog NEWS TODO
/%_lib/*.so.*
%_bindir/*
#/etc/profile.d/%name*

%files devel
%_includedir/*
/%_lib/%name.so
%_libdir/pkgconfig/*

%files devel-examples
%doc examples
#%files python-module-natspec
#%python_sitelibdir/*.so


%changelog
* Wed Feb 23 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.3-alt1
- new version (fix bug with unix charset)

* Mon Feb 21 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.2-alt1
- new version

* Sun Feb 20 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.1-alt1
- first public release (for ALT Linux Sisyphus)

