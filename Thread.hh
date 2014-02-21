// -*- mode: C++ -*-
// Time-stamp: "2012-10-23 10:19:46 sb"

/*
  file       Thread.hh
  copyright  (c) Sebastian Blatt 2012

  Compatibility header that loads either <pthread.h> or <thread>
  depending on whether we compile with -std=c++0x or -std=c++11.

 */


#ifndef THREAD_HH__ADFA5FD7_968E_4FE1_87D1_AFFBCC2EB396
#define THREAD_HH__ADFA5FD7_968E_4FE1_87D1_AFFBCC2EB396

// This test will only work on gcc >= 4.7, since otherwise
// __cplusplus has been defined as 1. See
//
//   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773
//
// However, this is fine since we will not want to compile with
// --std=c++0x or --std=c++11 below 4.7 anyway.
#if __cplusplus < 201100L
#define USING_PTHREAD 1
#endif

//#define USING_PTHREAD 1

#if USING_PTHREAD

extern "C" {
#include <pthread.h>
}

#else

#include <thread>

#endif // USING_PTHREAD


#endif // THREAD_HH__ADFA5FD7_968E_4FE1_87D1_AFFBCC2EB396

// Thread.hh ends here
