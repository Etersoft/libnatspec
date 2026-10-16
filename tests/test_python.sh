#!/bin/sh
# Public domain
# Tests for python binding (src/python)

PYDIR="${top_builddir:-..}/src/python"
[ -f "$PYDIR/.libs/_natspec.so" ] || { echo "skip: python binding is not built"; exit 77; }
# sanitizer runtime can not be loaded into python
if ldd "$PYDIR/.libs/_natspec.so" 2>/dev/null | grep -q "libasan" ; then
	echo "skip: python binding is built with sanitizer"
	exit 77
fi

PYTHONPATH="${top_srcdir:-..}/src/python:$PYDIR:$PYDIR/.libs" \
LD_LIBRARY_PATH="${top_builddir:-..}/lib/.libs${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
LC_ALL=C exec env -u LANG -u LC_CTYPE -u G_FILENAME_ENCODING "${PYTHON:-python3}" - <<'PYEOF'
import natspec as n

failed = 0
total = 0

def check(got, expected):
    global failed, total
    total += 1
    if got != expected:
        failed += 1
        import traceback
        line = traceback.extract_stack()[-2].lineno
        print("FAIL: line %d: got %r, expected %r" % (line, got, expected))

check(n.natspec_get_nls_by_charset("KOI8-R"), "koi8-r")
check(n.natspec_get_codepage_by_charset("IBM866"), 866)
check(n.natspec_humble_charset("koi8-r"), "KOI8R")
check(n.natspec_extract_charset_from_locale("ru_RU.UTF-8"), "UTF8")
check(n.natspec_get_charset_by_locale(n.NATSPEC_WINCS, "ru_RU.KOI8-R"), "CP1251")
check(n.natspec_get_charset_by_locale(n.NATSPEC_DOSCS, "ru_RU.KOI8-R"), "IBM866")
check(n.natspec_get_charset_by_charset(n.NATSPEC_WINCS, n.NATSPEC_UNIXCS, "KOI8-R"), "CP1251")
check(n.natspec_locale_is_utf8("ru_RU.UTF-8"), 1)
check(n.natspec_locale_is_utf8("ru_RU.KOI8-R"), 0)
check(n.natspec_get_filename_encoding("ru_RU.KOI8-R"), "koi8-r")
check(n.natspec_convert("Привет", "ASCII", "UTF-8", 1), "Privet")
check(n.natspec_convert_with_translit("Ж", "ASCII", "UTF-8"), "ZH")
check(n.natspec_get_enriched_fs_options("ntfs", "nls=utf8"), "nls=utf8")

print("%d checks, %d failed" % (total, failed))
raise SystemExit(1 if failed else 0)
PYEOF
