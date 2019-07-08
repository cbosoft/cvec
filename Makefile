CC 		= gcc
CFLAGS 	= -O3 -g
LINK	= -lm -fopenmp

SO		=	libcvec.so
MAN   = cvec.7.gz

HDR		= src/cvec.h

OBJ 	= src/vector.o \
			 	src/int_vector.o \
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

src/%.o: src/%.c $(HDR)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC -c $<

%.so: $(OBJ) $(HDR)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC $(OBJ)

cvec.7.gz: cvec.7
	gzip -k cvec.7

install: shared cvec.7.gz
	cp $(HDR) /usr/include/.
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
	rm -f tests/*test **/*.o **/*.so vgcore* $(MAN)
