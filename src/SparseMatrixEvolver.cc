// -*- mode: C++ -*-
// Time-stamp: "2015-08-20 21:19:27 sb"

/*
  file       SparseMatrixEvolver.cc
  copyright  (c) Sebastian Blatt 2015

 */


#include <iostream>
#include <cassert>
#include <cstring>

#include <gsl/gsl_matrix.h>

#include "GSLWrappedCall.hh"
#include "SparseMatrixEvolver.hh"


int ode_f(double, const double* y, double* f, void* params){
  assert(y);
  assert(f);
  assert(params);

  const Sparse::SparseMatrix* p = reinterpret_cast< Sparse::SparseMatrix* >(params);
  assert(p);

  p->MultiplyByColumnVector(y, p->Columns(), f, p->Rows());

  return GSL_SUCCESS;
}

// FIXME: This is not efficient, need to outsource writing of dfdy to
// Sparse matrix class. More importantly, the algorithms need to decompose the
// resulting dfdy matrix at each step which is super-inefficient because not optimized
// for sparse matrices at all.
//
// Stay with methods that do not require the Jacobian!
int ode_df(double, const double* y, double* dfdy, double* dfdt, void* params){
  assert(y);
  assert(dfdy);
  assert(dfdt);
  assert(params);

  const Sparse::SparseMatrix* p = reinterpret_cast< Sparse::SparseMatrix* >(params);
  assert(p);
  size_t n_rows = p->Rows();
  size_t n_columns = p->Columns();

  gsl_matrix_view dfdy_view = gsl_matrix_view_array(dfdy, n_rows, n_columns);
  gsl_matrix* dfdy_m = &dfdy_view.matrix;

  for(size_t i=0; i<n_rows; ++i){
    for(size_t j=0; j<n_columns; ++j){
      gsl_matrix_set(dfdy_m, i, j, p->GetUnsafe(i, j));
    }
  }
  memset(dfdt, 0, sizeof(double) * n_rows);

  return GSL_SUCCESS;
}



SparseMatrixEvolver::SparseMatrixEvolver(Sparse::SparseMatrix& M_)
  : M(M_),
    N(M_.Rows()),
    t(0),
    h(1e-3),
    ctl_abs_err(1e-6), // default value in Matlab, see odeset
    ctl_rel_err(1e-3), // default value in Matlab, see odeset
    // ode45 = Dormand-Prince (4,5), rk8pd = Dormand-Prince (8,9)
    // maybe rkf45 is the most similar?
    step_type(gsl_odeiv2_step_rk8pd),
    //step_type(gsl_odeiv2_step_rkf45),
    //step_type(gsl_odeiv2_step_bsimp),
    ode_stepper(NULL),
    ode_controller(NULL),
    ode_evolver(NULL)
{
  ode_stepper = gsl_odeiv2_step_alloc(step_type, N);
  ode_controller = gsl_odeiv2_control_y_new(ctl_abs_err, ctl_rel_err);
  ode_evolver = gsl_odeiv2_evolve_alloc(N);
  ode_system.function = ode_f;
  ode_system.jacobian = ode_df;
  ode_system.dimension = N;
  ode_system.params = (void*)(&M);
}

SparseMatrixEvolver::~SparseMatrixEvolver(){
  gsl_odeiv2_evolve_free(ode_evolver);
  gsl_odeiv2_control_free(ode_controller);
  gsl_odeiv2_step_free(ode_stepper);
}

double SparseMatrixEvolver::Evolve(std::vector<double>& y0, const double tmax){
  while(t < tmax){
    GSLCALL(gsl_odeiv2_evolve_apply,
            ode_evolver,
            ode_controller,
            ode_stepper,
            &ode_system,
            &t,
            tmax,
            &h,
            &(y0[0]));
  }
  return t;
}

void SparseMatrixEvolver::Reset(){
  t = 0;
  h = 1e-3;
}


// SparseMatrixEvolver.cc ends here
