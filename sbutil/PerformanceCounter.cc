// -*- mode: C++ -*-
// Time-stamp: "2014-02-23 12:00:50 sb"

/*
  file       PerformanceCounter.cc
  copyright  (c) Sebastian Blatt 2014

 */


#include <sbutil/PerformanceCounter.hh>

#include <cmath>

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

  // Good reference for timing
  //
  //   https://github.com/bittorrent/libutp/blob/master/utp_utils.cpp

  frequency = CLOCKS_PER_SEC;
  counter_start = std::clock();
#endif // SBUTIL_IS_PLATFORM_WINDOWS

  time_start = (unsigned long)time(0);
}

double PerformanceCounter::GetRelativeTime(){
#if SBUTIL_IS_PLATFORM_WINDOWS

  LARGE_INTEGER tmp;
  QueryPerformanceCounter(&tmp);
  return ((double)(tmp.QuadPart - counter_start)) / frequency;

#else

  return ((double)(std::clock() - counter_start)) / frequency;

#endif // SBUTIL_IS_PLATFORM_WINDOWS
}

double PerformanceCounter::GetAbsoluteTime(){
  double rel = GetRelativeTime();
  return (double)time_start + rel;
}

unsigned long PerformanceCounter::GetAbsoluteTimeRounded(){
  // Use floor instead of round to ensure that we are never in the future.
  double rel = floor(GetRelativeTime());
  return time_start + (unsigned long)rel;
}

unsigned long PerformanceCounter::GetStartTime(){
  return (unsigned long)time_start;
}

// PerformanceCounter.cc ends here
