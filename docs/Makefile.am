DOC_CFG = *Dox.cfg
# оперделяется в configure.ac
# DOXYGEN = doxygen

DIRS = html
DOCFILES = ${DIRS}/*.{html,css,png}
########################################################################

all: ${DOC_CFG}
	${DOXYGEN} $<

depend :

.PHONY: clean
clean:
	${RM} ${DOCFILES} *~

re: clean all


# install
docdir = $(datadir)/doc/@PACKAGE@-@VERSION@/html
doc_DATA = $(shell find $(top_srcdir)/docs/html -name \*.*) 
