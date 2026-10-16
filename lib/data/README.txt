Warning!
We use bsearch for search by first column of arrays. The data
must be sorted in first column.

Manual changes of Windows/DOS/Mac charsets are kept in charset_overrides.txt
and applied by gen_data.sh (apply_overrides.sh) after generation, so they
are not lost when the table is regenerated.

Notes:
 - For uk_UA DOS codepage IBM866 is used as in Windows (OEM code page 866
   in Wine tools/make_unicode), not CP1125: Linux kernel has no cp1125 nls,
   so codepage=1125 can not be used for vfat
 - For be_BE DOS codepage changed to IBM849
