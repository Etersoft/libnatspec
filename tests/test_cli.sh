#!/bin/sh
# Public domain
# Tests for natspec command line utility

NATSPEC="${top_builddir:-..}/src/natspec"

[ -x "$NATSPEC" ] || { echo "skip: $NATSPEC is not built"; exit 77; }
if "$NATSPEC" -V 2>&1 | grep -q "without popt" ; then
	echo "skip: natspec is built without popt"
	exit 77
fi

failed=0
total=0

# run natspec in clean environment with given variables
run()
{
	env -i PATH="$PATH" TMPDIR="$TMPDIR" \
		${ASAN_OPTIONS+ASAN_OPTIONS="$ASAN_OPTIONS"} \
		${UBSAN_OPTIONS+UBSAN_OPTIONS="$UBSAN_OPTIONS"} \
		"$@"
}

# check_out expected_output VAR=value... natspec args
check_out()
{
	expected="$1"
	shift
	total=$((total+1))
	got="$(run "$@" 2>&1)"
	if [ "$got" != "$expected" ] ; then
		echo "FAIL: $*: got '$got', expected '$expected'"
		failed=$((failed+1))
	fi
}

# check_rc expected_exit_code VAR=value... natspec args
check_rc()
{
	expected="$1"
	shift
	total=$((total+1))
	run "$@" >/dev/null 2>&1
	got=$?
	if [ "$got" != "$expected" ] ; then
		echo "FAIL: $*: exit code $got, expected $expected"
		failed=$((failed+1))
	fi
}

KOI=LC_ALL=ru_RU.KOI8-R
UTF=LC_ALL=ru_RU.UTF-8

check_out "ru_RU.KOI8-R" $KOI "$NATSPEC" --locale
check_out "ru_RU.UTF-8" $UTF "$NATSPEC" -l
check_out "koi8-r" $KOI "$NATSPEC" --fsenc
check_out "utf8" $UTF "$NATSPEC" -f
check_out "cp1251" $UTF G_FILENAME_ENCODING=CP1251 "$NATSPEC" --fsenc
check_out "866" $KOI "$NATSPEC" --codepage
check_out "437" LC_ALL=en_US "$NATSPEC" -c
check_out "KOI8R" $KOI "$NATSPEC" --charset unix
check_out "CP1251" $KOI "$NATSPEC" --charset win
check_out "CP1251" $KOI "$NATSPEC" --charset WIN
check_out "IBM866" $KOI "$NATSPEC" -s dos
check_out "MACCYRILLIC" $KOI "$NATSPEC" -s mac
check_out "ASCII" $KOI "$NATSPEC" --charset nonexistent
check_rc 1 $KOI "$NATSPEC" --charset nonexistent
check_rc 0 $UTF "$NATSPEC" --utf8
check_rc 1 $KOI "$NATSPEC" --utf8
check_out "Privet" LC_ALL=C "$NATSPEC" --transl UTF-8 "Привет"
check_rc 0 $UTF "$NATSPEC" --info
check_rc 0 "$NATSPEC" --help

total=$((total+1))
if ! run "$NATSPEC" -V | grep -q "^libnatspec [0-9]" ; then
	echo "FAIL: natspec -V does not print version"
	failed=$((failed+1))
fi

echo "$total checks, $failed failed"
[ "$failed" = 0 ]
