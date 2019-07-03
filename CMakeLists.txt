cmake_minimum_required (VERSION 2.6)
project (cvec)

# dependencies
find_library(fftw3 REQUIRED)

set(SRC src/vector.c src/filter.c src/cvec.h   src/io.c   src/sort.c  src/error.c  src/fourier.c  src/int_vector.c  src/matrices.c  src/stats.c  src/signalproc.c  src/limits.c)

# targets
add_library(cvec SHARED ${SRC})

# install
install(TARGETS cvec DESTINATION /usr/lib)
install(FILES src/cvec.h DESTINATION /usr/include)
