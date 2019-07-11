CC 		= gcc
CFLAGS 	= -O3 -g
LINK	= -lm -fopenmp

SO		=	libcvec.so
MAN   = cvec.7

HDR 	= src/cvec.h \
				src/stats_template.h \
				src/sort_template.h \
				src/vector_template.h

TEMPLATES = src/stats_template.c \
						src/sort_template.c \
						src/vector_template.c

OBJ 	= src/vector.o \
			 	src/int_vector.o \
			 	src/uint_vector.o \
				src/fourier.o \
				src/sort.o \
				src/int_sort.o \
				src/stats.o \
				src/matrices.o \
				src/io.o \
				src/filter.o \
				src/error.o \
				src/limits.o \
				src/signalproc.o

TESTS = tests/vector_creation_test

.SECONDARY:

default: shared

shared: $(SO) $(HDR) $(TEMPLATES)

src/%.o: src/%.c $(HDR) src/%_template.c src/%_template.h
	$(CC) $(CFLAGS) -shared -o $@ -fPIC -c $<

src/int_%.o: src/int_%.c $(HDR) src/%_template.c src/%_template.h
	$(CC) $(CFLAGS) -shared -o $@ -fPIC -c $<

%.so: $(OBJ) $(HDR)
	$(CC) $(CFLAGS) -shared -o $@ -fPIC $(OBJ)

install: $(SO) $(HDR) $(MAN)
	cp $(HDR) /usr/include/.
	cp $(MAN) /usr/share/man/man7/.
	mv $(SO) /usr/lib/.

uninstall:
	rm -f /usr/include/$(HDR)
	rm -f /usr/share/man/man7/$(MAN)
	rm -f /usr/lib/$(SO)

test: $(TESTS)
	tests/vector_creation_test

%test: %test.c
	$(CC) $(CFLAGS) -o $@ $< -lcvec $(LINK)

clean:
	rm -f tests/*test **/*.o **/*.so vgcore*
