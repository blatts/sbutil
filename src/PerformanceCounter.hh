// -*- mode: C++ -*-
// Time-stamp: "2014-02-22 22:29:57 sb"

/*
  file       PerformanceCounter.hh
  copyright  (c) Sebastian Blatt 2014

 */


#ifndef PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F
#define PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

#include "Platform.hh"

#if SBUTIL_IS_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else

#include <ctime>
#include <sys/time.h>

#endif //SBUTIL_IS_PLATFORM_WINDOWS

class PerformanceCounter {
  private:
    double frequency;
 #if SBUTIL_IS_PLATFORM_WINDOWS
    __int64 counter_start;
 #else
    unsigned long counter_start;
 #endif // SBUTIL_IS_PLATFORM_WINDOWS
    time_t time_start;

  public:
    PerformanceCounter();
    double GetRelativeTime();
    double GetAbsoluteTime();
    unsigned GetStartTime();
};

#endif // PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

// PerformanceCounter.hh ends here
