#! /bin/sh

# We run just autoreconf, updates all needed
autoreconf -fiv
./configure --enable-maintainer-mode --with-debug --with-python CFLAGS="-Werror -pedantic"
