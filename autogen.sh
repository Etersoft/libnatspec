#! /bin/sh

set -x
aclocal
autoheader
libtoolize --force --copy
automake --add-missing --include-deps --copy --force-missing
autoconf

