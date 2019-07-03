cmake_minimum_required (VERSION 2.6)
project (cvec)

# dependencies
find_library(fftw3 REQUIRED)

set(SRC src/cvec.c src/cvec_filter.c src/cvec.h   src/cvec_io.c   src/cvec_sort.c  src/cvec_error.c  src/cvec_fourier.c  src/cvec_int.c  src/cvec_matrix.c  src/cvec_stats.c  src/cvec_signal.c  src/cvec_limits.c)

# targets
add_library(cvec SHARED ${SRC})

# install
install(TARGETS cvec DESTINATION /usr/lib)
install(FILES src/cvec.h DESTINATION /usr/include)
