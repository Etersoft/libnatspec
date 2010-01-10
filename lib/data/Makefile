# for console only program
CFLAGS=-mwindows
#-mconsole

# -I /home/lav/Project/wine/include/
LIBUNICODE=
# -lwine_unicode

PROG=print_data_string

GCC=winegcc

$(PROG): $(PROG).c Makefile
	$(GCC) $(CFLAGS) $< -o $@ $(LIBUNICODE)
