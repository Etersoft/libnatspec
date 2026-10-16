#!/bin/sh
# This caused GLib2 applications to convert filenames from
# G_FILENAME_ENCODING encoding to UTF-8.
# Any application can use G_FILENAME_ENCODING for this purposes
# or link with natspec library.

NATSPEC=/usr/bin/natspec
test -x $NATSPEC && export G_FILENAME_ENCODING=`$NATSPEC -f`
