// -*- mode: C++ -*-
// Time-stamp: "2014-02-23 12:13:23 sb"

/*
  file       PerformanceCounter.hh
  copyright  (c) Sebastian Blatt 2014

  General purpose code timing. Wraps QueryPerformanceCounter on
  Windows and std::clock otherwise. Not sure how much to trust the
  accuracy of these counters for long intervals.

  FIXME: Try to find a relative interval accuracy estimate dtau/tau of
  std::clock. Once we have reached dtau/tau * tau = 1s, switch to
  time() instead. Also should take into account that clock_t overflows
  around 36 minutes. There is also a microsecond gettimeofday
  function. Assume that std::clock is significantly faster to execute.

  See these articles

    http://nadeausoftware.com/articles/2012/03/c_c_tip_how_measure_cpu_time_benchmarking
    http://nadeausoftware.com/articles/2012/04/c_c_tip_how_measure_elapsed_real_time_benchmarking

 */


#ifndef PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F
#define PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

#include <sbutil/Platform.hh>

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
    clock_t counter_start;
 #endif // SBUTIL_IS_PLATFORM_WINDOWS
    unsigned long time_start;

  public:
    PerformanceCounter();
    double GetRelativeTime();
    double GetAbsoluteTime();
    unsigned long GetAbsoluteTimeRounded();
    unsigned long GetStartTime();
};

#endif // PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

// PerformanceCounter.hh ends here
