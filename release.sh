#!/bin/sh
# Make release tarball libnatspec-VERSION.tar.gz from a git tag
# with generated configure (by make distcheck, so it is checked to build).
#
# Usage: ./release.sh [tag]     (default tag is the version from configure.ac)
# Result: ../libnatspec-VERSION.tar.gz
#
# Publish it on GitHub Releases, so distributions do not need autotools:
#   gh release create VERSION ../libnatspec-VERSION.tar.gz -R Etersoft/libnatspec

set -e

tag="$1"
[ -n "$tag" ] || tag=$(sed -n 's/^AC_INIT(\[libnatspec\], \[\([^]]*\)\].*/\1/p' configure.ac)
[ -n "$tag" ] || { echo "Can't get version from configure.ac" >&2 ; exit 1 ; }
outdir=$(cd .. && pwd)

tmp=$(mktemp -d)
trap 'chmod -R u+w "$tmp" ; rm -rf "$tmp"' EXIT

git archive --prefix=src/ "$tag" | tar xf - -C "$tmp"
cd "$tmp/src"
autoreconf -fi
./configure
make distcheck

for i in libnatspec-*.tar.gz ; do
	cp -v "$i" "$outdir/"
	ls -l "$outdir/$i"
done
