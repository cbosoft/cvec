CC 		= gcc
CFLAGS 	= -O0 -g
SRC 	= cvec.c cvec_int.c cvec_fourier.c cvec_sort.c
SO		=	libcvec.so
HDR		= cvec.h
LINK	= -lcvec -lm -lcgnuplot -lfftw3 -fopenmp

shared: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -shared -o $(SO) -fPIC $(SRC)

install: shared
	cp $(HDR) /usr/include/.
	mv $(SO) /usr/lib/.

uninstall:
	rm -f /usr/include/$(HDR)
	rm -f /usr/lib/$(SO)

tests: omptest

omptest: omptest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)

ffttest: ffttest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)

clean:
	rm -f *test PLOT.png *.o *.so vgcore* gplt_tmp*
