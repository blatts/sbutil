// -*- mode: C++ -*-
// Time-stamp: "2014-10-15 18:25:16 sb"

/*
  file       GSLMatrix.cc
  copyright  (c) Sebastian Blatt 2014

 */

#include "GSLMatrix.hh"
#include "Exception.hh"
#include <sstream>

// -------------------------------------------------------------------- GSLVector

void GSLVector::Cleanup(){
  if(vector){
    gsl_vector_free(vector);
    vector = NULL;
  }
}

void GSLVector::Copy(const gsl_vector* x){
  assert(x);

  Cleanup();

  vector = gsl_vector_alloc(x->size);
  if(!vector){
    std::ostringstream os;
    os << "gsl_vector_alloc(" << x->size << ") failed.";
    throw EXCEPTION(os.str());
  }

  gsl_vector_memcpy(vector, x);
}


GSLVector::GSLVector(size_t dim)
  : vector(NULL)
{
  vector = gsl_vector_calloc(dim);
  if(!vector){
    std::ostringstream os;
    os << "gsl_vector_calloc(" << dim << ") failed.";
    throw EXCEPTION(os.str());
  }
}

GSLVector::GSLVector(size_t dim, double default_value)
  : vector(NULL)
{
  vector = gsl_vector_alloc(dim);
  if(!vector){
    std::ostringstream os;
    os << "gsl_vector_alloc(" << dim << ") failed.";
    throw EXCEPTION(os.str());
  }

  gsl_vector_set_all(vector, default_value);
}

GSLVector::GSLVector(const gsl_vector* vector_)
  : vector(NULL)
{
  Copy(vector_);
}

GSLVector::GSLVector(const GSLVector& x)
  : vector(NULL)
{
  Copy(x.GetPointer());
}

GSLVector& GSLVector::operator=(const GSLVector& x){
  Copy(x.GetPointer());
  return *this;
}

GSLVector& GSLVector::operator=(const gsl_vector* vector_){
  Copy(vector_);
  return *this;
}

GSLVector::~GSLVector(){
  Cleanup();
}

std::ostream& GSLVector::Represent(std::ostream& out) const {
  assert(vector);
  for(size_t i=0; i<vector->size; ++i){
    out << ((i == 0) ? "[ " : "");
    out << gsl_vector_get(vector, i);
    out << ((i == vector->size - 1) ? " ]" : "\t");
  }
  return out;
}


// -------------------------------------------------------------------- GSLMatrix


void GSLMatrix::Cleanup(){
  if(matrix){
    gsl_matrix_free(matrix);
    matrix = NULL;
  }
}

void GSLMatrix::Copy(const gsl_matrix* x){
  assert(x);
  Cleanup();

  matrix = gsl_matrix_alloc(x->size1, x->size2);
  if(!matrix){
    std::ostringstream os;
    os << "gsl_matrix_alloc(" << x->size1 << "," << x->size2
       << ") failed.";
    throw EXCEPTION(os.str());
  }
  gsl_matrix_memcpy(matrix, x);
}


GSLMatrix::GSLMatrix(size_t dim1, size_t dim2)
  : matrix(NULL)
{
  matrix = gsl_matrix_calloc(dim1, dim2);
  if(!matrix){
    std::ostringstream os;
    os << "gsl_matrix_calloc(" << dim1 << "," << dim2
       << ") failed.";
    throw EXCEPTION(os.str());
  }
}

GSLMatrix::GSLMatrix(size_t dim1, size_t dim2, double default_value)
  : matrix(NULL)
{
  matrix = gsl_matrix_alloc(dim1, dim2);
  if(!matrix){
    std::ostringstream os;
    os << "gsl_matrix_alloc(" << dim1 << "," << dim2
       << ") failed.";
    throw EXCEPTION(os.str());
  }
  gsl_matrix_set_all(matrix, default_value);
}

GSLMatrix::GSLMatrix(const gsl_matrix* matrix_)
  : matrix(NULL)
{
  Copy(matrix_);
}

GSLMatrix::GSLMatrix(const GSLMatrix& x)
  : matrix(NULL)
{
  Copy(x.GetPointer());
}

GSLMatrix& GSLMatrix::operator=(const GSLMatrix& x){
  Copy(x.GetPointer());
  return *this;
}

GSLMatrix& GSLMatrix::operator=(const gsl_matrix* matrix_){
  Copy(matrix_);
  return *this;
}

GSLMatrix::~GSLMatrix(){
  Cleanup();
}

std::ostream& GSLMatrix::Represent(std::ostream& out) const {
  assert(matrix);
  for(size_t i=0; i<matrix->size1; ++i){ // rows
    out << ((i == 0) ? "/ " : ((i == matrix->size1 - 1) ? "\\ " : "| "));
    for(size_t j=0; j<matrix->size2; ++j){ // columns
      out << gsl_matrix_get(matrix, i, j);
      if(j < matrix->size2-1){
        out << "\t";
      }
    }
    out << ((i == 0) ? " \\\n" : ((i == matrix->size1 - 1) ? " /\n" : " |\n"));
  }
  return out;
}


// GSLMatrix.cc ends here
