DLIST=lib src tests

if HAVE_DOXYGEN
SUBDIRS = $(DLIST) docs
else
SUBDIRS = $(DLIST)
endif

# install for devel
pkgconfigdir 	= $(libdir)/pkgconfig
pkgconfig_DATA 	= libnatspec.pc

m4dir = $(datadir)/aclocal
m4_DATA = natspec.m4

man_MANS = man/natspec.1

