// -*- mode: C++ -*-
// Time-stamp: "2014-02-25 16:31:24 sb"

/*
  file       Timestamp.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */


#include <cstring>
#include <cerrno>
#include <cmath>
#include <sys/time.h>
#include <sstream>

#include "Timestamp.hh"
#include "OutputManipulator.hh"
#include "Exception.hh"

Timestamp::Timestamp()
  : Representable(),
    stamp(0u),
    microseconds(0u)
{
  Now();
}

std::ostream& Timestamp::Represent(std::ostream& out) const {
  tm x;
  memset(&x, 0, sizeof(x));
  if(localtime_r((time_t*)&stamp, &x)){
    out << right_justified<int>(1900 + x.tm_year, 4, '0') << "-"
        << right_justified<int>(1 + x.tm_mon, 2, '0') << "-"
        << right_justified<int>(x.tm_mday, 2, '0') << " "
        << right_justified<int>(x.tm_hour, 2, '0') << ":"
        << right_justified<int>(x.tm_min, 2, '0') << ":"
        << right_justified<int>(x.tm_sec, 2, '0') << ":"
        << microseconds / 1000 << ":"
        << microseconds % 1000 << " "
        << tzname[1];
  }
  return out;
}

void Timestamp::Now() {
  struct timeval t;
  struct timezone tz;
  memset(&t, 0, sizeof(t));
  memset(&tz, 0, sizeof(tz));
  if(gettimeofday(&t, &tz) != 0){
    std::ostringstream os;
    os << "gettimeofday(2) failed.\n"
       << "  " << strerror(errno);
    throw EXCEPTION(os.str());
  }
  else{
    stamp = t.tv_sec;
    microseconds = t.tv_usec;
  }
}

void Timestamp::At(int year, int month, int day,
                   int hour, int minute, int second,
                   int msecond, int usecond)
{
  struct tm t;
  memset(&t, 0, sizeof(t));
  time_t s = 0;
  if(localtime_r(&s, &t) == NULL){
    std::ostringstream os;
    os << "localtime_r(3) failed.\n"
       << "  " << strerror(errno);
    throw EXCEPTION(os.str());
  }

  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;
  t.tm_isdst = -1;
  time_t rc = 0;
  if((rc = mktime(&t)) == -1){
    std::ostringstream os;
    os << "mktime(3) failed.";
    throw EXCEPTION(os.str());
  }
  stamp = rc;
  microseconds = msecond * 1000 + usecond;
}




RelativeTime::RelativeTime(double offset_)
  : offset(offset_)
{
}

std::ostream& RelativeTime::Represent(std::ostream& out) const {
  out << offset;
  return out;
}

RelativeTime operator-(const Timestamp& a, const Timestamp& b){
  double s = ((double)a.GetSeconds()) - ((double)b.GetSeconds());
  double u = ((double)a.GetMicroSeconds()) - ((double)b.GetMicroSeconds());
  return RelativeTime(s + u*1e-6);
}

Timestamp operator+(const Timestamp& a, const RelativeTime& b) {
  double dt = b.Get();
  double s = floor(dt);
  return Timestamp(a.GetSeconds() + s, (dt - s) * 1e6);
}


// Timestamp.cc ends here
