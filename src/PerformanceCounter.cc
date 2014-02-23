// -*- mode: C++ -*-
// Time-stamp: "2014-02-22 22:29:24 sb"

/*
  file       PerformanceCounter.cc
  copyright  (c) Sebastian Blatt 2014

 */


#include "PerformanceCounter.hh"

PerformanceCounter::PerformanceCounter()
  : frequency(0),
    counter_start(0)
{

#if SBUTIL_IS_PLATFORM_WINDOWS

  LARGE_INTEGER tmp;
  QueryPerformanceFrequency(&tmp);
  frequency = ((double)tmp.QuadPart);

  QueryPerformanceCounter(&tmp);
  counter_start = tmp.QuadPart;

#else

  // FIXME: This currently punts to time(). Equivalent timing
  // implementation as on WIN32 needs to be done for DARWIN and
  // posix platforms! Good reference
  //
  // https://github.com/bittorrent/libutp/blob/master/utp_utils.cpp
  //
  // Second option should use new C++11 std::clock stuff.

  frequency = 1;
  counter_start = time(0);
#endif // SBUTIL_IS_PLATFORM_WINDOWS

  time_start = time(0);
}

double PerformanceCounter::GetRelativeTime(){
#if SBUTIL_IS_PLATFORM_WINDOWS

  LARGE_INTEGER tmp;
  QueryPerformanceCounter(&tmp);
  return ((double)(tmp.QuadPart - counter_start)) / frequency;

#else

  // FIXME: This currently punts to time(). Equivalent timing
  // implementation as on WIN32 needs to be done for DARWIN and
  // posix platforms!

  return ((double)(time(0) - counter_start)) / frequency;

#endif // SBUTIL_IS_PLATFORM_WINDOWS
}

double PerformanceCounter::GetAbsoluteTime(){
  return ((unsigned long)time_start) + GetRelativeTime();
}

unsigned PerformanceCounter::GetStartTime(){
  return (unsigned)time_start;
}

// PerformanceCounter.cc ends here
