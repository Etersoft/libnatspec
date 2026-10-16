DLIST=lib src tests

if HAVE_DOXYGEN
SUBDIRS = $(DLIST) docs
else
SUBDIRS = $(DLIST)
endif

DIST_SUBDIRS = $(DLIST) docs libhi

# install for devel
pkgconfigdir 	= $(libdir)/pkgconfig
pkgconfig_DATA 	= libnatspec.pc

m4dir = $(datadir)/aclocal
m4_DATA = natspec.m4

dist_man_MANS = man/natspec.1

EXTRA_DIST = README-ru.html examples profile libnatspec.spec
