sbutil
======

Collection of C++ utilities.

Build a static library from the files in the lib/ subdirectory. When
linking against files, need to also include libraries.

Potential libraries needed

  * m (should be included when compiling with C++)
  * pthread (unless compiled with C++11)
  * boost_regex (unless compiled with C++11)
  * gsl (numerics)
  * gslcblas (numerics)
  * hdf5 (HDF5 files)
