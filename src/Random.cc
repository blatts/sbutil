// -*- mode: C++ -*-
// Time-stamp: "2016-06-07 13:14:51 sb"

/*
  file       Random.cc
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */

#include "Random.hh"
#include "Rotation.hh"
#include "OutputManipulator.hh"
#include "VectorTools.hh"

#include <cmath>
#include <ctime>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

struct rng_impl{
    gsl_rng* p;
    rng_impl()
      : p(NULL)
    {
      p = gsl_rng_alloc(gsl_rng_default);
    }
    ~rng_impl(){
      if(p){
        gsl_rng_free(p);
        p = NULL;
      }
    }
};

Random::Random()
  : rng(NULL)
{
  rng = new rng_impl;
}

Random::~Random(){
  if(rng){
    delete rng;
    rng = NULL;
  }
}

void Random::SeedWithTimeNull(){
  gsl_rng_set(rng->p, (unsigned long)time(0));
}


double Random::Uniform(double lo, double hi){
  return gsl_ran_flat(rng->p, lo, hi);
}

double Random::Normal(double sigma, double mu){
  return gsl_ran_gaussian_ziggurat(rng->p, sigma) + mu;
}

double Random::Exponential(double mu){
  return gsl_ran_exponential(rng->p, mu);
}

size_t Random::Poisson(double mu){
  return gsl_ran_poisson(rng->p, mu);
}

double Random::StudentT(double nu){
  return gsl_ran_tdist(rng->p, nu);
}




Point Random::UniformOnSphericalSurface(double radius){
  double t = 0.0;
  double v[3] = {0.0, 0.0, 0.0};
  for(size_t i=0; i<3; ++i){
    v[i] = Normal(1.0);
    t += v[i]*v[i];
  }
  t = sqrt(t);
  for(size_t i=0; i<3; ++i){
    v[i] *= radius / t;
  }
  return Point(v[0], v[1], v[2]);
}

Point Random::UniformOnHalfsphereAround(const Point& normal){
  // Create random pointing in upper half sphere
  Point new_direction = UniformOnSphericalSurface();
  new_direction.z = fabs(new_direction.z);
  // Rotate upper half sphere to surface normal
  return Rotation(normal) * new_direction;
}

Point Random::CosOnHalfsphereAround(const Point& normal) {
  double phi = Uniform(0, 2 * M_PI);
  double s_theta = sqrt(Uniform());
  double c_theta = sqrt(1.0 - s_theta*s_theta);

  Point new_direction(cos(phi) * s_theta,
                      sin(phi) * s_theta,
                      c_theta);
  return Rotation(normal) * new_direction;
}


DiscreteDistribution::DiscreteDistribution(Random& rnd_,
                                           std::vector<double>& probabilities_)
  : rnd(rnd_),
    pdf(probabilities_.size(), 0.0),
    cdf(probabilities_.size(), 0.0)
{
  // normalize probabilities to make sure
  copy(pdf, probabilities_);
  normalize_by_total(pdf);

  // integrate pdf -> cdf
  copy(cdf, pdf);
  for(size_t i=1; i<cdf.size(); ++i){
    cdf[i] += cdf[i-1];
  }
}

size_t DiscreteDistribution::GetRandomStep() {
  const size_t jmax = cdf.size();
  const double x = rnd.Uniform(); // cdf is normalized!
  size_t rc = 0;
  for(size_t j=0; j<jmax; ++j){
    if(cdf[j] >= x){
      rc = j;
      break;
    }
  }
  return rc;
}

std::ostream& DiscreteDistribution::Represent(std::ostream& out) const {
  out << "pdf : " << vector_form<double>(pdf) << "\n"
      << "cdf : " << vector_form<double>(cdf);
  return out;
}

void Test__DiscreteDistribution(){
  std::vector<double> probs({0.20, 0.33, 0.47});

  Random rnd;
  rnd.SeedWithTimeNull();

  DiscreteDistribution P(rnd, probs);
  std::vector<int> hist(P.Size(), 0);
  const size_t N = 1000000;
  for(size_t i=0; i<N; ++i){
    size_t m = P.GetRandomStep();
    ++(hist[m]);
  }

  std::cout << vector_form<int>(hist) << std::endl;

  std::vector<double> percentages;
  copy(percentages, hist);
  scale(percentages, 100.0 / N);

  std::cout << vector_form<double>(percentages) << std::endl;
}



// Random.cc ends here
