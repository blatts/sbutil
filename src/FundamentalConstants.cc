// -*- mode: C++ -*-
// Time-stamp: "2014-02-25 17:33:01 sb"

/*
  file       FundamentalConstants.cc
  copyright  (c) Sebastian Blatt 2014

 */

#include "FundamentalConstants.hh"

#include <cmath>

#define SQR(x) ((x)*(x))

using namespace FundamentalConstants;

// -------------------------------------------------------- Fundamental constants

// Values obtained from Frink, see
//
//   https://futureboy.us/frinkdocs/


const double amu = 1.660538782e-27;
// could have used integer, but avoid confusion and errors by using double
const double c = 299792458;

const double electroncharge = 1.602176565e-19;

const double electronmass = 9.10938291e-31;

const double h = 6.62606896e-34;

const double kB = 1.3806503207853104e-23;

// ------------------------------------------------------- Mathematical constants

// Pi to machine precision can be calculated easily as
//
//   const double pi = 4.0 * atan(1.0);
//
// For compatibility with math.h, use the macro definition from the
// header here.

const double pi = M_PI;

// ----------------------------------------------------------- Derived quantities

// Derived quantities

const double mu0 = 4.0e-7 * FundamentalConstants::pi;

const double epsilon0 = 1.0 / (FundamentalConstants::mu0) / SQR(FundamentalConstants::c);

const double alpha = 0.25 / (FundamentalConstants::pi * FundamentalConstants::epsilon0) * \
                     SQR(FundamentalConstants::electroncharge) / \
                     (FundamentalConstants::hbar * FundamentalConstants::c);

const double hbar = FundamentalConstants::h/(2.0 * FundamentalConstants::pi);

const double bohrradius = FundamentalConstants::hbar / FundamentalConstants::electronmass / \
                          FundamentalConstants::c / FundamentalConstants::alpha;





// FundamentalConstants.cc ends here
