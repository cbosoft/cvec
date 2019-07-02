# cvec

`c` vector maths library.

# Installation

Depends on `cmake` for building, and on `libfftw3` for fourier maths. If you've
got those installed, then `cvec` can easily be installed by cloning this repo
then:

```bash
cd build
cmake ..
make
sudo make install
```

Done! Ready to do some maths.

# Overview

To make things a bit easy to change in the future, there're two new types
define: `cvec_float` and `cvec_int`. The former is a double, and the latter is a
long long int. This may change in the future. All functions in cvec use these
types.

Vectors are implemented as  pointer arrays of `cvec_float` (or `cvec_int`) and
are passed to functions with their length. Functions are named whether they deal
with floats or ints. `cvec_<name>` is a float function, and `cvec_int_<name>` is
an int function.

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

And more. Check out the header for all funcs.
