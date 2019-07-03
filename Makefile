CC 		= gcc
CFLAGS 	= -O0 -g
LINK	= -lm -lfftw3 -fopenmp

SO		=	build/libcvec.so
HDR		= src/cvec.h
SRC 	= src/vector.c \
			 	src/int_vector.c \
				src/fourier.c \
				src/sort.c \
				src/stats.c \
				src/matrices.c \
				src/io.c \
				src/filter.c \
				src/error.c \
				src/limits.c \
				src/signalproc.c
TESTS = tests/omptest \
				tests/ffttest \
				tests/sorttest \
				tests/mattest \
				tests/fittest \
				tests/filtertest

default: install

shared: builddir $(SO)

builddir:
	mkdir -p build

%.so: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC $(SRC)

install: shared
	sudo cp $(HDR) /usr/include/.
	sudo mv $(SO) /usr/lib/.

uninstall:
	sudo rm -f /usr/include/$(HDR)
	sudo rm -f /usr/lib/$(SO)

test: build_tests
	tests/omptest
	tests/ffttest
	tests/sorttest
	tests/mattest
	tests/fittest
	tests/filtertest

build_tests: $(TESTS)

%test: %test.c $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(SRC) $(LINK)

clean:
	rm -f tests/*test PLOT.png **/*.o **/*.so vgcore* gplt_tmp*
