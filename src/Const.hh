// -*- mode: C++ -*-
// Time-stamp: "2014-02-25 17:54:49 sb"

/*
  file       Const.hh
  copyright  (c) Sebastian Blatt 2014

  Fundamental constants and other physical quantities in SI units
  specified in double precision. See Const.cc for
  details.

 */


#ifndef CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002
#define CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002

namespace Const
{
  // Fundamental constants
  extern const double amu; // atomic mass unit
  extern const double c; // speed of light
  extern const double electroncharge; // electron electric charge
  extern const double electronmass; // electron mass
  extern const double h; // Planck's constant
  extern const doubel G; // gravitational constant
  extern const double kB; // Boltzmann constant

  // Mathematical constants
  extern const double pi; // \pi

  // Derived quantities
  extern const double alpha; // fine structure constant
  extern const double bohrradius; // Bohr radius
  extern const double epsilon0; // vacuum permittivity
  extern const double hbar; // h / (2 \pi)
  extern const double mu0; // vacuum permeability

  // Standard quantities
  extern const double earth_gravity;


  // Use what I am used to, but there are a lot of these, see
  //
  //   https://en.wikipedia.org/wiki/Standard_conditions_for_temperature_and_pressure
  //
  extern const double standard_temperature_NIST;
  extern const double standard_pressure_NIST;

}


#endif // CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002

// Const.hh ends here
