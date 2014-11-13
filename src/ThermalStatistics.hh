// -*- mode: C++ -*-
// Time-stamp: "2014-11-13 10:23:19 sb"

/*
  file       ThermalStatistics.hh
  copyright  (c) Sebastian Blatt 2014

  Thermal statistics. Use this mostly to generate a vector of
  probabilities for a vector of energies.

 */


#ifndef THERMALSTATISTICS_HH__2AF7DA4C_EA64_11E3_B240_283737241892
#define THERMALSTATISTICS_HH__2AF7DA4C_EA64_11E3_B240_283737241892

#include "Representable.hh"

#include <vector>

// Leave the bare functions accessible when not using
// ProbabilitesFromEnergies() function below.

double BoltzmannFactor(double energy, double temperature);
double BoseEinsteinFactor(double energy, double temperature, double chemical_potential);
double FermiDiracFactor(double energy, double temperature, double chemical_potential);



// Adapt the above into an energy functor to generalize the idea. This
// seems very bloaty, really I just wanted curried function pointers.
class EnergyStatistics : public Representable {
  public:
    EnergyStatistics() {}
    virtual double operator()(const double energy) const = 0;
};

class Boltzmann : public EnergyStatistics {
  public:
    const double temperature;
    Boltzmann(double temperature_ = 0.0);
    double operator()(const double energy) const;
    std::ostream& Represent(std::ostream& out) const;
};

class BoseEinstein : public EnergyStatistics {
  public:
    const double temperature;
    const double chemical_potential;
    BoseEinstein(double temperature_ = 0.0, double chemical_potential_ = 0.0);
    double operator()(const double energy) const;
    std::ostream& Represent(std::ostream& out) const;
};

class FermiDirac : public EnergyStatistics {
  public:
    const double temperature;
    const double chemical_potential;
    FermiDirac(double temperature_ = 0.0, double chemical_potential_ = 0.0);
    double operator()(const double energy) const;
    std::ostream& Represent(std::ostream& out) const;
};


// normalize = true: normalize to partition function cut off at size
// of energy vector.
//
// normalize = false: bare Boltzmann factor without partition function

void ProbabilitiesFromEnergies(const std::vector<double>& energies,
                               const EnergyStatistics& statistics,
                               bool normalize,
                               std::vector<double>& probabilities);



#endif // THERMALSTATISTICS_HH__2AF7DA4C_EA64_11E3_B240_283737241892

// ThermalStatistics.hh ends here
