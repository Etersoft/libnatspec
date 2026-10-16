DOC_CFG = *Dox.cfg
# оперделяется в configure.ac
# DOXYGEN = doxygen

DIRS = html
########################################################################

all-local: ${DOC_CFG}
	${DOXYGEN} $<

clean-local:
	rm -rf ${DIRS}

# install
htmldocdir = $(datadir)/doc/@PACKAGE@-@VERSION@/html

install-data-local:
	$(MKDIR_P) $(DESTDIR)$(htmldocdir)
	$(INSTALL_DATA) ${DIRS}/* $(DESTDIR)$(htmldocdir)

uninstall-local:
	rm -rf $(DESTDIR)$(htmldocdir)
