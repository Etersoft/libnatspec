#!/bin/sh
# Run fuzzers built by build.sh, each for SECONDS (default 60).
# Usage: tests/fuzz/run.sh [BUILDDIR [SECONDS]]
#   SECONDS=0 only runs the inputs of the corpus (regression check)
#   New interesting inputs are kept in BUILDDIR/fuzz/corpus-NAME,
#   crashes are saved to BUILDDIR/fuzz/crash-NAME-*
set -e

src=$(cd "$(dirname "$0")/../.." && pwd)
build=$(cd "${1:-.}" && pwd)
seconds=${2:-60}
out="$build/fuzz"
rc=0
if [ "$seconds" = 0 ] ; then
	limit="-runs=0"
else
	limit="-max_total_time=$seconds"
fi

# leaks are not checked: natspec utility does not free on exit
export ASAN_OPTIONS=detect_leaks=0:abort_on_error=1

for f in "$out"/fuzz_* ; do
	name=$(basename "$f" | sed 's/^fuzz_//')
	case "$f" in *.o) continue ;; esac
	corpus="$out/corpus-$name"
	mkdir -p "$corpus"
	echo "=== $name: $seconds s"
	if ! "$f" $limit -timeout=10 -rss_limit_mb=2048 \
		-artifact_prefix="$out/crash-$name-" \
		"$corpus" "$src/tests/fuzz/corpus/$name" 2>"$out/$name.log" ; then
		echo "FAIL: $name, see $out/$name.log and $out/crash-$name-*"
		tail -30 "$out/$name.log"
		rc=1
	else
		grep -E "^#[0-9]+.*DONE|Done [0-9]+ runs" "$out/$name.log" | tail -1
	fi
done
exit $rc
