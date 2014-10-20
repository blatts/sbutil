// -*- mode: C++ -*-
// Time-stamp: "2014-10-15 01:19:09 sb"

/*
  file       Random.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */

#include "Random.hh"
#include "Rotation.hh"

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
  static const Point ez = Point(0,0,1);
  Point new_direction = UniformOnSphericalSurface();
  new_direction.z = fabs(new_direction.z);
  // Rotate upper half sphere to surface normal
  Rotation rot;
  rot.FromTo(ez, normal);
  return rot * new_direction;
}

Point Random::CosOnHalfsphereAround(const Point& normal) {
  Point rc;
  while(true){
    rc = UniformOnHalfsphereAround(normal);
    double angle = normal.Dot(rc);
    if(Uniform() <= cos(angle)){
      break;
    }
  }
  return rc;
}


DiscreteDistribution::DiscreteDistribution(Random& rnd_,
                                           std::vector<double>& probabilities_)
  : rnd(rnd_),
    pdf(probabilities_.size(), 0.0),
    cdf(probabilities_.size(), 0.0)
{
  // normalize probabilities to make sure
  std::copy(probabilities_.begin(), probabilities_.end(), pdf.begin());
  double ptot = 0.0;
  for(size_t i=0; i<pdf.size(); ++i){
    ptot += pdf[i];
  }
  for(size_t i=0; i<pdf.size(); ++i){
    pdf[i] /= ptot;
  }
  // integrate pdf -> cdf
  std::copy(pdf.begin(), pdf.end(), cdf.begin());
  for(size_t i=1; i<cdf.size(); ++i){
    cdf[i] += cdf[i-1];
  }
}

size_t DiscreteDistribution::GetRandomStep() {
  const size_t jmax = cdf.size() - 1;
  const double x = rnd.Uniform(cdf[0], cdf[jmax]);
  for(size_t j=1; j<jmax; ++j){
    if(cdf[j] >= x){
      return j;
    }
  }
  // should not get here
  return jmax;
}



// Random.cc ends here
