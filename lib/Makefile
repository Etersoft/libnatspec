CFLAGS = -funsigned-char -g
LIB = -lpopt

GCC=gcc

.c.o:
	${GCC} ${CFLAGS} -c $<

all: test_get_charset natspec
	./test_get_charset

test_get_charset: test_get_charset.o get_charset.o
	gcc $^ ${LIB} -o $@

natspec: natspec.o get_charset.o
	gcc $^ ${LIB} -o $@
