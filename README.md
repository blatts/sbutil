sbutil
======

Collection of C++ utilities.

Download this at your own peril. This project is meant to be useful to
me. If you find something useful within as well, good for you.

We use scons to build a static library from the files in the lib/
subdirectory. When linking against files, also need to link with
libraries (see below). The code makes an effort to be as portable as I
have needed so far. The platforms parts of the code have been run on
are Windows 7 Professional, OSX Lion 10.7.5, and Gentoo linux.

Potential libraries needed

  * m (should be included when compiling with C++)
  * pthread (unless compiled with C++11)
  * boost_regex (unless compiled with C++11)
  * gsl (numerics)
  * gslcblas (numerics)
  * hdf5 (HDF5 files)
