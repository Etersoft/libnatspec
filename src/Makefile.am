if HAVE_PYTHON
SUBDIRS = python
endif

bin_PROGRAMS 	= natspec
natspec_SOURCES = natspec.c
natspec_LDADD 	= $(top_srcdir)/lib/libnatspec.la $(POPT_LIBS)
natspec_CFLAGS	= -I$(top_srcdir)/lib -I$(top_srcdir)/libhi
