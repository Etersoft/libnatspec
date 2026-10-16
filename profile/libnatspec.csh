#!/bin/csh
# This caused GLib2 applications to convert filenames from
# G_FILENAME_ENCODING encoding to UTF-8.
# Any other application can use G_FILENAME_ENCODING for this purposes
# or link with natspec library.

setenv NATSPEC /usr/bin/natspec
test -x $NATSPEC && setenv G_FILENAME_ENCODING `$NATSPEC -f`
