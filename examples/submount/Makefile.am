
sbin_PROGRAMS = submountd net-submountd

submountd_SOURCES = submountd.c mount_guess_fstype.c
submountd_LDADD = $(LIB_RESMGR) $(NATSPEC_LIBS)
net_submountd_SOURCES = net-submountd.c
man_MANS = submount.8

EXTRA_DIST = linux_fs.h mount_guess_fstype.c submountd.c submountd.h \
net-submountd.c net-submountd.h $(man_MANS)
