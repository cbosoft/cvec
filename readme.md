# cvec

`c` vector maths library.

# Installation

Depends only on a c compiler. I've used `GNU make` for making the build process
easy, but there's nothing GNU-exclusive in the source, this should be portable
even to windows, but I've not tried it.

Compile and build from terminal:

```bash
make && sudo make install
```

Done! Ready to do some maths.

# Uninstallation

If you want to uninstall, `cvec` only installs three files to your system:
library, header, and manual. These are installed as `/usr/lib/libcvec.so`,
`/usr/include/cvec.h`, and `/usr/share/man/man7/cvec.7.gz`. You can remove them
using the Makefile:

```bash
sudo make uninstall
```

# Overview

`cvec` uses its own types for everything: `cvec_float`, `cvec_int` and
`cvec_uint`. Vectors are implemented as  pointer arrays of `cvec_float` (or
`cvec_int`) and are passed to functions with their length (`cvec_uint`).
Functions are named whether they deal with floats or ints. `cvec_<name>` is a
float function, and `cvec_int_<name>` is an int function.

Highlights:

  - `cvec_apply`

    Apply a function to every element of the vector, creating a second vector
    with the results. If possible, this is done in parallel.

  - `cvec_apply2`

    Apply a function to two vectors together, creating a third vector with the
    results. If possible, this is done in parallel.

  - `cvec_bubble_sort`

    Sort the vector into ascending order

  - `cvec_min`, `cvec_max`

    Get the lowest (or highest) value in the vector

  - `cvec_diff`

    Difference between each element and the subsequent element in a vector.

  - `cvec_fft`, `cvec_freq`

    Fourier transform of data in vector. This is just a nice wrapper around the
    excellent `fftw3` library.

And more. Check out the man page, or the header for more information.

To read the manual without installing:

```bash
man -l cvec.7
```
