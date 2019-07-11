CC 		= gcc
CFLAGS 	= -O3 -g
LINK	= -lm -fopenmp

SO		=	libcvec.so
MAN   = cvec.7

HDRSRC = src/cvec.h
HDR    = inst.h

OBJ 	= src/vector.o \
			 	src/int_vector.o \
			 	src/uint_vector.o \
				src/fourier.o \
				src/sort.o \
				src/stats.o \
				src/matrices.o \
				src/io.o \
				src/filter.o \
				src/error.o \
				src/limits.o \
				src/signalproc.o

TESTS = tests/omptest \
				tests/ffttest \
				tests/sorttest \
				tests/mattest \
				tests/fittest \
				tests/filtertest

.SECONDARY:

default: shared

shared: $(SO)

inst.h: $(HDRSRC)
	echo "#pragma once" > $@
	cpp $< >> $@

src/%.o: src/%.c $(HDRSRC)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC -c $<

%.so: $(OBJ) $(HDRSRC)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC $(OBJ)

install: $(SO) $(HDR) $(MAN)
	cp $(HDR) /usr/include/cvec.h
	cp $(MAN) /usr/share/man/man7/.
	mv $(SO) /usr/lib/.

uninstall:
	rm -f /usr/include/$(HDR)
	rm -f /usr/share/man/man7/$(MAN)
	rm -f /usr/lib/$(SO)

test: build_tests
	tests/omptest
	tests/ffttest
	tests/sorttest
	tests/mattest
	tests/fittest
	tests/filtertest

build_tests: $(TESTS)

%test: %test.c $(OBJ:.o=.c)
	$(CC) $(CFLAGS) -o $@ $< $(OBJ:.o=.c) $(LINK)

clean:
	rm -f tests/*test **/*.o **/*.so vgcore*
