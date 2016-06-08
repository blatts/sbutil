// -*- mode: C++ -*-
// Time-stamp: "2015-08-20 22:23:13 sb"

/*
  file       GSLWrappedCall.hh
  copyright  (c) Sebastian Blatt 2014, 2015


  Define an Exception throwing convenience wrapper to validate all GSL
  calls that return an integer code validatable against GSL_SUCCESS.

  The idea is that we can write calls to GSL functions using the
  GSLCALL() macro defined below and rely on the wrapper to
  automatically throw an Exception containing the location of the
  error and the GSL error message from gsl_strerror.



  WORK IN PROGRESS. MAKE THIS GENERAL AND SAFE.

  The default error handler prints the error string and aborts. We
  want to turn this off and implement our own exception based
  reporting with line numbers and so on.

  From GSL documentation:

  gsl_*_alloc calls the GSL error handler when insufficient memory is
  available, and set the error code to GSL_ENOMEM. Thus we do not have
  to check every alloc.

  gsl_*_set has bounds checking (unless compiled with
  GSL_RANGE_CHECK_OFF) and will call the error handler.


 */


#ifndef GSLWRAPPEDCALL_HH__241BAC80_549A_11E4_9ED8_283737241892
#define GSLWRAPPEDCALL_HH__241BAC80_549A_11E4_9ED8_283737241892

#include <sbutil/WrappedCall.hh>
#include <gsl/gsl_errno.h>

inline void InitializeGSLErrorReporting(){
  gsl_set_error_handler_off();
}

class ValidatorGSL : public Validator<int> {
  public:
    bool Validate(const int& x) const {return x == GSL_SUCCESS;}
    std::ostream& InvalidMessage(const int& x, std::ostream& out) const {
      const std::string n = typeid(int).name();
      out << n << " (" << x << " == GSL_SUCCESS) returned false.\n"
          << "gsl_strerror: \"" << gsl_strerror(x) << "\"";
      return out;
    }
};

#define GSLCALL(f, ...) WCALL(f, ValidatorGSL, __VA_ARGS__)


#endif // GSLWRAPPEDCALL_HH__241BAC80_549A_11E4_9ED8_283737241892

// GSLWrappedCall.hh ends here
