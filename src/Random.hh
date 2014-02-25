// -*- mode: C++ -*-
// Time-stamp: "2013-02-04 20:41:25 sb"

/*
  file       Random.hh
  copyright  (c) Sebastian Blatt 2012, 2013

 */


#ifndef RANDOM_HH__B0F37B3C_5E4D_4D3E_A97A_C2353945BAB6
#define RANDOM_HH__B0F37B3C_5E4D_4D3E_A97A_C2353945BAB6

#include "Point.hh"
#include <vector>

// use pimpl to hide random number generator interna
struct rng_impl;
class Random {
  private:
    rng_impl* rng;
  public:
    Random();
    ~Random();

    void SeedWithTimeNull();

    double Uniform(double lo = 0.0, double hi = 1.0);
    double Normal(double sigma = 1.0, double mu = 0.0);
    Point UniformOnSphericalSurface(double radius = 1.0);
    Point UniformOnHalfsphereAround(const Point& normal);
    Point CosOnHalfsphereAround(const Point& normal);
};

class DiscreteDistribution {
  private:
    Random& rnd;
    std::vector<double> pdf;
    std::vector<double> cdf;
  public:
    DiscreteDistribution(Random& rnd_, std::vector<double>& probabilities_);
    size_t GetRandomStep();
};


#endif // RANDOM_HH__B0F37B3C_5E4D_4D3E_A97A_C2353945BAB6

// Random.hh ends here
