// -*- mode: C++ -*-
// Time-stamp: "2016-06-08 14:40:06 sb"

/*
  file       ThermalStatistics.cc
  copyright  (c) Sebastian Blatt 2014, 2015, 2016

 */

#include <sbutil/ThermalStatistics.hh>
#include <sbutil/VectorTools.hh>

#include <sbutil/Const.hh>

#include <cmath>

double BoltzmannFactor(double energy, double temperature) {
  return exp(-energy / (Const::kB * temperature));
}

double BoseEinsteinFactor(double energy, double temperature, double chemical_potential){
  return 1.0 / ( exp( (energy - chemical_potential)/(Const::kB * temperature)) - 1.0);
}

double FermiDiracFactor(double energy, double temperature, double chemical_potential){
  return 1.0 / ( exp( (energy - chemical_potential)/(Const::kB * temperature)) + 1.0);
}


// ------------------------------------------------------------- EnergyStatistics


void EnergyStatistics::Probabilities(const std::vector<double>& energies,
                                     bool normalize,
                                     std::vector<double>& probabilities)
{
  const size_t N = energies.size();
  probabilities.resize(N);

  for(size_t i=0; i<N; ++i){
    probabilities[i] = (*this)(energies[i]);
  }

  if(normalize){
    normalize_by_total(probabilities);
  }
}



// -------------------------------------------------------------------- Boltzmann

Boltzmann::Boltzmann(double temperature_)
  : temperature(temperature_)
{}

double Boltzmann::operator()(const double energy) const {
  return BoltzmannFactor(energy, temperature);
}

std::ostream& Boltzmann::Represent(std::ostream& out) const{
  out << "Boltzmann distribution at T = "
      << temperature;
  return out;
}

// ----------------------------------------------------------------- BoseEinstein

BoseEinstein::BoseEinstein(double temperature_, double chemical_potential_)
  : temperature(temperature_),
    chemical_potential(chemical_potential_)
{}

double BoseEinstein::operator()(const double energy) const {
  return BoseEinsteinFactor(energy, temperature, chemical_potential);
}


std::ostream& BoseEinstein::Represent(std::ostream& out) const{
  out << "Bose-Einstein distribution at T = "
      << temperature << ", mu = " << chemical_potential;
  return out;
}

// ------------------------------------------------------------------- FermiDirac



FermiDirac::FermiDirac(double temperature_, double chemical_potential_)
  : temperature(temperature_),
    chemical_potential(chemical_potential_)
{}

double FermiDirac::operator()(const double energy) const {
  return FermiDiracFactor(energy, temperature, chemical_potential);
}

std::ostream& FermiDirac::Represent(std::ostream& out) const{
  out << "Fermi-Dirac distribution at T = "
      << temperature << ", mu = " << chemical_potential;
  return out;
}




// ThermalStatistics.cc ends here
