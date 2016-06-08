// -*- mode: C++ -*-
// Time-stamp: "2014-10-15 14:31:12 sb"

/*
  file       WrappedCall.hh
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

  Defines a convenience macro

    WCALL(f, v, ...)

  where F is a pointer to a function with arbitrary arguments and
  arbitrary return type R. The type V is a validator that defines

    bool V::operator()(const R&) const;

  which, when called, determines the validity of a value of type R
  returned by F. If the validity check returns false, an Exception is
  thrown that contains information about the function name, arguments,
  file, line number and parent function.

  This file relies on the variadic template argument lists defined in
  C++11 and it needs to be compiled with -std=c++0x.

  Otherwise the header will default to using a pure macro based
  version and will emit a compiler warning.

  Main difference: the macro will not return the validated return
  value from applying the wrapped function!

 */


#ifndef WRAPPEDCALL_HH__64354C65_9BFB_484F_B89D_E93092E10735
#define WRAPPEDCALL_HH__64354C65_9BFB_484F_B89D_E93092E10735

// This test will only work on gcc >= 4.7, since otherwise
// __cplusplus has been defined as 1. See
//
//   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=1773
//
// However, this is fine since we will not want to compile with
// --std=c++0x or --std=c++11 below 4.7 anyway.
#if __cplusplus < 201100L
#else
#define HAVE_VARIADIC_TEMPLATES 1
#endif

#include <sstream>
#include <iostream>
#include <sbutil/Exception.hh>
#include <sbutil/Validator.hh>

#if HAVE_VARIADIC_TEMPLATES

#include <utility>

namespace metamagic
{

template<typename T>
std::ostream& print_list(std::ostream& out, T&& value){
  out << value;
  return out;
}
template<typename First, typename... Rest>
std::ostream& print_list(std::ostream& out, First&& first, Rest&&... rest){
  out << first << ", ";
  return print_list(out, std::forward<Rest>(rest)...);
}


template<typename Sig, Sig& S, typename Val>
struct wrapper;

template<typename Ret, typename... Args, Ret(&P)(Args...), typename Val>
struct wrapper<Ret(Args...), P, Val> {
    static Ret apply(const std::string& WRAPPED_FUNCTION,
                     const std::string& FILE,
                     const int LINE,
                     const std::string& FUNCTION,
                     Args... args)
    {
      Ret r = P(args...);
      Val v;
      if(!v(r)){
        std::ostringstream os;
        os << "Function call " << WRAPPED_FUNCTION << "(";
        print_list(os, args...);
        os << ") failed return value validation:\n";
        v.InvalidMessage(r, os);
        throw Exception(os.str(), FILE, LINE, FUNCTION);
      }
      return r;
    }

};

}

#define WCALL(f, v, ...) metamagic::wrapper<decltype(f), f, v>::apply(#f, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#else // HAVE_VARIADIC_TEMPLATES


#warning "WCALL() used without C++11 variadic template argument lists. Not typesafe. No return value!!!"

// Emulate above behavior as good as possible using a block and a macro.
#define WCALL(f, v, ...)  {                                             \
    v val;                                                              \
    v::validated_type r = f(__VA_ARGS__);                               \
    if(!val(r)){                                                        \
      std::ostringstream os;                                            \
      os << "Function call " << #f << "("                               \
         << #__VA_ARGS__ << ") failed return value validation:\n";      \
      val.InvalidMessage(r, os);                                        \
      throw Exception(os.str(), __FILE__, __LINE__, __FUNCTION__);      \
    }                                                                   \
  }

#endif // HAVE_VARIADIC_TEMPLATES

#endif // WRAPPEDCALL_HH__64354C65_9BFB_484F_B89D_E93092E10735

// WrappedCall.hh ends here
