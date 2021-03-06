// -*- mode: C++ -*-
// Time-stamp: "2015-08-18 18:38:57 sb"

/*
  file       Const.hh
  copyright  (c) Sebastian Blatt 2014, 2015

  Fundamental constants and other physical quantities in SI units
  specified in double precision. See Const.cc for
  details.

 */


#ifndef CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002
#define CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002

namespace Const
{
  // Fundamental constants
  extern const double avogadro; // Avogadro number
  extern const double amu; // atomic mass unit
  extern const double c; // speed of light
  extern const double electroncharge; // electron electric charge
  extern const double electronmass; // electron mass
  extern const double h; // Planck's constant
  extern const double G; // gravitational constant
  extern const double kB; // Boltzmann constant

  // Mathematical constants
  extern const double pi; // \pi

  // Derived quantities
  extern const double alpha; // fine structure constant
  extern const double bohrradius; // Bohr radius
  extern const double epsilon0; // vacuum permittivity
  extern const double hbar; // h / (2 \pi)
  extern const double mu0; // vacuum permeability
  extern const double muB; // Bohr magneton

  // Standard quantities

  extern const double standard_gravity;

  // Use what I am used to, but there are a lot of thermodynamics
  // conventions, see
  //
  //   https://en.wikipedia.org/wiki/Standard_conditions_for_temperature_and_pressure
  //
  extern const double standard_temperature_NIST;
  extern const double standard_pressure_NIST;

}


#endif // CONST_HH__18958542_606F_4D4D_9A94_0F5ADD754002

// Const.hh ends here
