#! /bin/sh

set -x
#./cvs2cl.pl -btT
aclocal
autoheader
libtoolize --force --copy
automake --add-missing --include-deps --copy --force-missing
autoconf
test -f Makefile && ./configure --with-debug --with-python
