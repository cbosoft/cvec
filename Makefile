CC 		= gcc
CFLAGS 	= -O0 -g
SRC 	= cvec.c cvec_int.c cvec_fourier.c cvec_sort.c cvec_stats.c cvec_matrix.c
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

tests: omptest ffttest sorttest mattest

omptest: omptest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)

ffttest: ffttest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
	
sorttest: sorttest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)
	
mattest: mattest.c
	$(CC) $(CFLAGS) -o $@ $< $(LINK)

clean:
	rm -f *test PLOT.png *.o *.so vgcore* gplt_tmp*
