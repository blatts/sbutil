// -*- mode: C++ -*-
// Time-stamp: "2014-02-21 14:17:25 sb"

/*
  file       PerformanceCounter.cc
  copyright  (c) Sebastian Blatt 2014

 */


#include "PerformanceCounter.hh"

#ifdef WIN32
#include <windows.h>
#endif

PerformanceCounter::PerformanceCounter()
  : frequency(0),
    counter_start(0)
{
#ifdef WIN32
  LARGE_INTEGER tmp;
  QueryPerformanceFrequency(&tmp);
  frequency = ((double)tmp.QuadPart);

  QueryPerformanceCounter(&tmp);
  counter_start = tmp.QuadPart;
#else
  frequency = 1;
  counter_start = time(0);
#endif //WIN32
  time_start = time(0);
}

double PerformanceCounter::GetRelativeTime(){
#ifdef WIN32
  LARGE_INTEGER tmp;
  QueryPerformanceCounter(&tmp);
  return ((double)(tmp.QuadPart - counter_start)) / frequency;
#else
  return ((double)(time(0) - counter_start)) / frequency;
#endif // WIN32
}

double PerformanceCounter::GetAbsoluteTime(){
  return ((unsigned long)time_start) + GetRelativeTime();
}

unsigned PerformanceCounter::GetStartTime(){
  return (unsigned)time_start;
}

// PerformanceCounter.cc ends here
