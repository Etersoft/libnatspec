
if HAVE_PYTHON
pyexec_LTLIBRARIES = _natspec.la
else
pyexec_LTLIBRARIES = 
endif

_natspec_la_SOURCES = natspec_wrap.c
_natspec_la_CFLAGS = $(LIBNATSPEC_CFLAGS) $(PYTHON_INCLUDES) $(PYTHON_QUIET)
_natspec_la_LDFLAGS = -module -avoid-version $(LIBNATSPEC)

pyexec_SCRIPTS = natspec.py

EXTRA_DIST = natspec.i README.txt natspec.py setup.py test_natspec.py

#cd python
#CFLAGS="$RPM_OPT_FLAGS" %__python setup.py build
#%__python setup.py install --root $RPM_BUILD_ROOT
