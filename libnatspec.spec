Name: libnatspec
Version: 0.0.2
Release: alt1

Summary: Nation & languages specifity issues library

License: LGPL
Group: Development/C
Url: http://www.etersoft.ru

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: http://www.etersoft.ru/download/%name-%version.tar.bz2

# manually removed: gcc-g77  xorg-x11-libs
# Automatically added by buildreq on Sun Feb 20 2005
BuildRequires: gcc-c++ hostinfo libpopt-devel libstdc++-devel

%description
Nation & languages specifity issues library
This library provides helper functions for
	mount, submount

%package devel
Summary: The files needed for %name application development
Group: Development/C
Requires: %name = %version-%release

%description devel
The %name-devel package contains the necessary include files
for developing applications with %name

%prep
%setup -q

%build
%configure --libdir=/%_lib
%make_build

%install
%makeinstall

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

%changelog
* Mon Feb 21 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.2-alt1
- new version

* Sun Feb 20 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.1-alt1
- first public release (for ALT Linux Sisyphus)

