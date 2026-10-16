#!/bin/sh
# Apply manual overrides from charset_overrides.txt to generated table
# Usage: apply_overrides.sh [get_charset_data.h [charset_overrides.txt]]
#   Every override line is: locale windows_cs dos_cs mac_cs [# comment]
#   It changes rows for locale and all its variants (locale.charset, locale@modifier)

FILE=${1:-get_charset_data.h}
OVERRIDES=${2:-$(dirname "$0")/charset_overrides.txt}

awk -v OVR="$OVERRIDES" '
function trim(s) { gsub(/^[ \t]+|[ \t]+$/, "", s); return s }
function quote(s) { return s == "NULL" ? " NULL" : "\"" s "\"" }
# the same formatting as print_w in print_data_string.c
function pw(s, w,   t) { t = s (w ? ", " : ""); while (length(t) < w) t = t " "; return t }
BEGIN {
	FS = ","
	while ((getline line < OVR) > 0) {
		sub(/#.*/, "", line)
		if (split(line, a, " ") != 4)
			continue
		win[a[1]] = a[2]; dos[a[1]] = a[3]; mac[a[1]] = a[4]
	}
}
/^ \{"/ {
	key = $0; sub(/^ \{"/, "", key); sub(/".*/, "", key)
	base = key; sub(/[.@].*/, "", base)
	if (base in win) {
		loc = trim($1); sub(/^\{/, "", loc)
		printf(" {%s%s%s%s%s%s },\n", pw(loc, 16), pw(trim($2), 7), pw(trim($3), 15),
			pw(quote(win[base]), 12), pw(quote(dos[base]), 12), pw(quote(mac[base]), 0))
		next
	}
}
{ print }
' "$FILE" > "$FILE.tmp" && mv "$FILE.tmp" "$FILE"
