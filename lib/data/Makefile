# for window only program
#CFLAGS=-mwindows

# for console only program
CFLAGS=
#-mconsole
# -I /home/lav/Project/wine/include/
LIBUNICODE=
# -lwine_unicode

PROG=print_data_string

#GCC=i386-mingw32msvc-g++
GCC=winegcc

$(PROG): $(PROG).c Makefile
	$(GCC) $< $(CFLAGS) -o $@ $(LIBUNICODE)
