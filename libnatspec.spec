Name: libnatspec
Version: 0.0.1
Release: alt0.1

Summary: Nation & languages specifity issues library

License: LGPL
Group: Development/C
Url: http://www.etersoft.ru

Packager: Vitaly Lipatov <lav@altlinux.ru>

Source: http://www.etersoft.ru/download/%name-%version.tar.bz2

%description
Nation & languages specifity issues library

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
%configure
%make_build

%install
%makeinstall

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%doc AUTHORS COPYING README ChangeLog NEWS TODO
%_libdir/*.so.*
%_bindir/*

%files devel
%_includedir/%name
%_libdir/%name.so

%changelog
* Sun Feb 20 2005 Vitaly Lipatov <lav@altlinux.ru> 0.0.1-alt0.1
- first public release (for ALT Linux Sisyphus)

