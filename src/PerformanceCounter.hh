// -*- mode: C++ -*-
// Time-stamp: "2014-02-21 19:49:13 sb"

/*
  file       PerformanceCounter.hh
  copyright  (c) Sebastian Blatt 2014

 */


#ifndef PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F
#define PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#else

#include <ctime>
#include <sys/time.h>

#endif //WIN32

class PerformanceCounter {
  private:
    double frequency;
 #ifdef WIN32
    __int64 counter_start;
 #else
    unsigned long counter_start;
 #endif // WIN32
    time_t time_start;

  public:
    PerformanceCounter();
    double GetRelativeTime();
    double GetAbsoluteTime();
    unsigned GetStartTime();
};

#endif // PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

// PerformanceCounter.hh ends here
