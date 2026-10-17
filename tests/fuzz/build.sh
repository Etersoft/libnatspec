#!/bin/sh
# Build libFuzzer harnesses with clang, ASan and UBSan.
# Usage: tests/fuzz/build.sh [BUILDDIR]
#   BUILDDIR is a configured tree (with config.h, default: current directory),
#   the fuzzers are built to BUILDDIR/fuzz/
set -e

src=$(cd "$(dirname "$0")/../.." && pwd)
build=$(cd "${1:-.}" && pwd)
out="$build/fuzz"
mkdir -p "$out"

CC=${CC:-clang}
CFLAGS="-g -O1 -fsanitize=fuzzer,address,undefined -fno-sanitize-recover=undefined"
CPPFLAGS="-DHAVE_CONFIG_H -I$build -I$src/lib -I$src/libhi -I$src/tests"
LIB="$src/lib/get_charset.c $src/lib/enrich_fs_options.c $src/lib/convert.c \
	$src/lib/filesystem.c $src/lib/detect.c"
POPT_LIBS=$(sed -n 's/^POPT_LIBS = //p' "$build/src/Makefile")
LIBS="$POPT_LIBS -lpthread"

$CC $CFLAGS $CPPFLAGS -o "$out/fuzz_convert" "$src/tests/fuzz/fuzz_convert.c" $LIB "$src/lib/get_locale.c" $LIBS
$CC $CFLAGS $CPPFLAGS -o "$out/fuzz_names" "$src/tests/fuzz/fuzz_names.c" $LIB "$src/lib/get_locale.c" $LIBS
$CC $CFLAGS $CPPFLAGS -o "$out/fuzz_i18n" "$src/tests/fuzz/fuzz_i18n.c" $LIB "$src/tests/i18n_get_locale.c" $LIBS
if [ -n "$POPT_LIBS" ] ; then
	$CC $CFLAGS $CPPFLAGS -Dmain=natspec_main -Dexit=natspec_fuzz_exit \
		-c -o "$out/natspec_cli.o" "$src/src/natspec.c"
	$CC $CFLAGS $CPPFLAGS -o "$out/fuzz_cli" "$src/tests/fuzz/fuzz_cli.c" "$out/natspec_cli.o" \
		$LIB "$src/lib/get_locale.c" $LIBS
else
	echo "natspec is built without popt, skip fuzz_cli"
fi
ls -l "$out"/fuzz_*
