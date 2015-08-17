// -*- mode: C++ -*-
// Time-stamp: "2015-03-07 20:11:24 sb"

/*
  file       SparseMatrixEvolver.hh
  copyright  (c) Sebastian Blatt 2015

 */


#ifndef SPARSEMATRIXEVOLVER_HH__92041D9A_C484_11E4_87F1_283737241892
#define SPARSEMATRIXEVOLVER_HH__92041D9A_C484_11E4_87F1_283737241892

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include "Sparse.hh"

class SparseMatrixEvolver{
  public:
    Sparse::SparseMatrix& M;

    const size_t N;
    double t;
    double h;
    double ctl_abs_err;
    double ctl_rel_err;

    const gsl_odeiv2_step_type* step_type;
    gsl_odeiv2_step* ode_stepper;
    gsl_odeiv2_control* ode_controller;
    gsl_odeiv2_evolve* ode_evolver;
    gsl_odeiv2_system ode_system;

    SparseMatrixEvolver(Sparse::SparseMatrix& M_);
    ~SparseMatrixEvolver();

    double Evolve(std::vector<double>& y0, const double tmax);
    void Reset();
};



#endif // SPARSEMATRIXEVOLVER_HH__92041D9A_C484_11E4_87F1_283737241892

// SparseMatrixEvolver.hh ends here
