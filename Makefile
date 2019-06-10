CC 		= gcc
CFLAGS 	= -O0 -g
LINK	= -lcvec -lm -lcgnuplot -lfftw3 -fopenmp

SO		=	build/libcvec.so
HDR		= src/cvec.h
SRC 	= src/cvec.c \
			 	src/cvec_int.c \
				src/cvec_fourier.c \
				src/cvec_sort.c \
				src/cvec_stats.c \
				src/cvec_matrix.c \
				src/cvec_filter.c
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

%test: %test.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)

clean:
	rm -f tests/*test PLOT.png **/*.o **/*.so vgcore* gplt_tmp*
