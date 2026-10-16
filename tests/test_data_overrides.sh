#!/bin/sh
# Public domain
# lib/data/get_charset_data.h must already contain the manual overrides
# from lib/data/charset_overrides.txt (gen_data.sh applies them)

DATA="${top_srcdir:-..}/lib/data"
TMP="${TMPDIR:-/tmp}/natspec-test-overrides.$$"
trap 'rm -f "$TMP"' EXIT

cp "$DATA/get_charset_data.h" "$TMP" || exit 99
sh "$DATA/apply_overrides.sh" "$TMP" "$DATA/charset_overrides.txt" || exit 99
if ! cmp -s "$DATA/get_charset_data.h" "$TMP" ; then
	echo "FAIL: get_charset_data.h differs after applying charset_overrides.txt:"
	diff "$DATA/get_charset_data.h" "$TMP"
	exit 1
fi
echo "overrides are applied"
