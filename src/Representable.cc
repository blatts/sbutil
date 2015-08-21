// -*- mode: C++ -*-
// Time-stamp: "2015-08-18 13:51:41 sb"

/*
  file       Representable.cc
  copyright  (c) Sebastian Blatt 2011 -- 2015

 */

#include "Representable.hh"
#include "Platform.hh"

#include <ctime>
#include <cmath>

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

static const char* const english_numerals[20] =
{"zero", "one", "two", "three", "four", "five", "six", "seven",
 "eight", "nine",
 "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
 "sixteen", "seventeen", "eighteen", "nineteen"};

static const char* const english_decades[10] =
{"", "", "twenty", "thirty", "fourty", "fifty", "sixty", "seventy",
 "eighty", "ninety"};

// static const char* const english_magnitudes[] =
// {"", "thousand", "million", "billion", "trillion",
//  "quadrillion", "quintillion", "sixtillion", "septillion", "octillion",
//  "nonillion", "decillion", "undecillion", "duodecillion",
//   };

std::ostream& EnglishNumeral::Represent(std::ostream& out) const{
  if(x < 20){
    out << english_numerals[x];
  }
  // if(x < 100){
  //   out << english_decades[x/10] << "-" << english_numerals[x%10];
  //   return out;
  // }
  // if(x < 1000){
  //   out << english_numerals[x/100] << "-hundred ";
  //   size_t y = x % 100;
  //   out << english_decades[y/10] << "-" << english_numerals[y % 10];
  //   return out;
  // }

  // long double magnitude = log10l(static_cast<long double>(x));
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

// Representable.cc ends here
