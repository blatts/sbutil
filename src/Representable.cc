// -*- mode: C++ -*-
// Time-stamp: "2014-02-22 22:31:17 sb"

/*
  file       Representable.cc
  copyright  (c) Sebastian Blatt 2011, 2012, 2013, 2014

 */

#include "Representable.hh"
#include "Platform.hh"

#include <ctime>


std::ostream& operator<<(std::ostream& out, const Representable& e){
  if(out.good()){
    e.Represent(out);
  }
  return out;
}

std::ostream& Combination::Represent(std::ostream& out) const{
  out << a << b;
  return out;
}

std::ostream& Alternative::Represent(std::ostream& out) const{
  if(c){
    out << a;
  }
  else{
    out << b;
  }
  return out;
}

static const char* const english_numerals[10] =
{"zero", "one", "two", "three", "four", "five", "six", "seven",
 "eight", "nine"};

std::ostream& EnglishNumeral::Represent(std::ostream& out) const{
  if(x < 10){
    out << english_numerals[x];
  }
  else{
    out << x;
  }
  return out;
}

std::ostream& EnglishPluralize::Represent(std::ostream& out) const{
  out << s;
  if(m == 0 || m >= 2){
    out << "s";
  }
  return out;
}

// FIXME: This should be rebased on PerformanceCounter abstraction
// over timing information or C++11 std::clock.
std::ostream& TimeNow::Represent(std::ostream& out) const{
  time_t t = time(0);
#if SBUTIL_IS_PLATFORM_WINDOWS
  struct tm tmp;
  struct tm* p_tmp = &tmp;
  memset(p_tmp, 0, sizeof(struct tm));
  errno_t e = localtime_s(p_tmp, &t);
#else
  struct tm* p_tmp = NULL;
  p_tmp = localtime(&t);
#endif // SBUTIL_IS_PLATFORM_WINDOWS

  char s[20];
  strftime(s, 20, "%H:%M:%S", p_tmp);
  out << s;
  return out;
}

// Representable.cc ends here
