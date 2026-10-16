Warning!
We use bsearch for search by first column of arrays. The data
must be sorted in first column.

Manually changes (for WINE TODO):
 - For uk_UA DOS codepage IBM866 is used as in Windows (OEM code page 866
   in Wine tools/make_unicode), not CP1125: Linux kernel has no cp1125 nls,
   so codepage=1125 can not be used for vfat
 - For be_BE DOS codepage changed to IBM849
