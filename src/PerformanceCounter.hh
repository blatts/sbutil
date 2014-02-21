// -*- mode: C++ -*-
// Time-stamp: "2014-02-21 18:34:08 sb"

/*
  file       PerformanceCounter.hh
  copyright  (c) Sebastian Blatt 2014

 */


#ifndef PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F
#define PERFORMANCECOUNTER_HH__C9D42287_FFDA_45F5_9571_BF7F86011F8F

#include <ctime>

#ifdef WIN32
#include <windows.h>
#endif

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
