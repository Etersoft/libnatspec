#!/bin/bash

# Release project with autoreconf

# use etersoft-build-utils package
. /usr/share/eterbuild/eterbuild 1.7.0
load_mod spec


# git release script
tag=$1

# fast hack
SPEC=*.spec
version=$(get_version $SPEC)
pkgname=$(get_tarballname $SPEC)-$version
test -n "$tag" || tag=$(get_version $SPEC)-$(get_release $SPEC)
pwd=$(pwd)

git archive --prefix=$pkgname/ $tag | tar xvfC - $TMPDIR

cd $TMPDIR/$pkgname
if [ -f configure.ac ] ; then
	autoreconf -fiv
	rm -rf autom4te*
fi
cd ..
tar cfvj $pwd/../$pkgname.tar.bz2 $pkgname
rm -rf $pkgname
ls -l $pwd/../$pkgname.tar.bz2
