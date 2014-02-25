// -*- mode: C++ -*-
// Time-stamp: "2014-02-25 17:53:31 sb"

/*
  file       Const.cc
  copyright  (c) Sebastian Blatt 2014

 */

#include "Const.hh"

#include <cmath>

#define SQR(x) ((x)*(x))

using namespace Const;

// -------------------------------------------------------- Fundamental constants

// Values obtained from Frink, see
//
//   https://futureboy.us/frinkdocs/


const double amu = 1.660538782e-27;

// could have used integer, but avoid confusion and errors by using double
const double c = 299792458.0;

const double electroncharge = 1.602176565e-19;

const double electronmass = 9.10938291e-31;

const double h = 6.62606896e-34;

const double G = 6.67384e-11;

const double kB = 1.3806503207853104e-23;

// ------------------------------------------------------- Mathematical constants

// Pi to machine precision can be calculated easily as
//
//   const double pi = 4.0 * atan(1.0);
//
// For compatibility with math.h, use the macro definition from the
// header here. If desired, add more values as needed.
//
// #define M_E         2.71828182845904523536028747135266250   /* e */
// #define M_LOG2E     1.44269504088896340735992468100189214   /* log 2e */
// #define M_LOG10E    0.434294481903251827651128918916605082  /* log 10e */
// #define M_LN2       0.693147180559945309417232121458176568  /* log e2 */
// #define M_LN10      2.30258509299404568401799145468436421   /* log e10 */
// #define M_PI        3.14159265358979323846264338327950288   /* pi */
// #define M_PI_2      1.57079632679489661923132169163975144   /* pi/2 */
// #define M_PI_4      0.785398163397448309615660845819875721  /* pi/4 */
// #define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi */
// #define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi */
// #define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi) */
// #define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2) */
// #define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2) */

const double pi = M_PI;

// ----------------------------------------------------------- Derived quantities

// Derived quantities

const double mu0 = 4.0e-7 * Const::pi;

const double epsilon0 = 1.0 / (Const::mu0) / SQR(Const::c);

const double alpha = 0.25 / (Const::pi * Const::epsilon0) * \
                     SQR(Const::electroncharge) / \
                     (Const::hbar * Const::c);

const double hbar = Const::h/(2.0 * Const::pi);

const double bohrradius = Const::hbar / Const::electronmass / \
                          Const::c / Const::alpha;


// ---------------------------------------------------------- Standard quantities

const double earth_gravity = 9.81; // m/s^2

const double standard_temperature_NIST = 273.15; // K

const double standard_pressure_NIST = 101325; // Pa



// Const.cc ends here
