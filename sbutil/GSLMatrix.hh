// -*- mode: C++ -*-
// Time-stamp: "2014-10-20 15:09:13 sb"

/*
  file       GSLMatrix.hh
  copyright  (c) Sebastian Blatt 2014

  A thin wrapper around gsl_matrix and gsl_vector to get away from
  having to worry about alloc and free and simplify pretty-printing.

  Constructors and copy assignments memcpy the original matrix!
*/


#ifndef GSLMATRIX_HH__0CCDE9F0_54AC_11E4_B002_283737241892
#define GSLMATRIX_HH__0CCDE9F0_54AC_11E4_B002_283737241892

#include <cassert>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <sbutil/Representable.hh>

class GSLVector : public Representable {
  protected:
    gsl_vector* vector;

  private:
    void Cleanup();
    void Copy(const gsl_vector* x);

  public:
    GSLVector(size_t dim);
    GSLVector(size_t dim, double default_value);
    GSLVector(const gsl_vector* vector_);
    GSLVector(const GSLVector& x);
    GSLVector& operator=(const GSLVector& x);
    GSLVector& operator=(const gsl_vector* vector_);
    ~GSLVector();
    std::ostream& Represent(std::ostream& out) const;

    inline double Get(size_t i) const {
      assert(vector);
      return gsl_vector_get(vector, i);
    }

    inline GSLVector& Set(size_t i, double value){
      assert(vector);
      gsl_vector_set(vector, i, value);
      return *this;
    }
    inline GSLVector& SetAll(double value){
      assert(vector);
      gsl_vector_set_all(vector, value);
      return *this;
    }
    inline void Zeros() { SetAll(0.0); }

    inline size_t Length() const {
      assert(vector);
      return vector->size;
    }
    inline size_t Size() const {return Length();}

    inline const gsl_vector* GetPointer() const {
      assert(vector);
      return vector;
    }
    inline gsl_vector* GetBarePointer() {
      assert(vector);
      return vector;
    }
};


class GSLMatrix : public Representable {
  protected:
    gsl_matrix* matrix;

  private:
    void Cleanup();
    void Copy(const gsl_matrix* x);

  public:
    GSLMatrix(size_t dim1, size_t dim2);
    GSLMatrix(size_t dim1, size_t dim2, double default_value);
    GSLMatrix(const gsl_matrix* matrix_);
    GSLMatrix(const GSLMatrix& x);
    GSLMatrix& operator=(const GSLMatrix& x);
    GSLMatrix& operator=(const gsl_matrix* matrix_);
    ~GSLMatrix();
    std::ostream& Represent(std::ostream& out) const;

    inline double Get(size_t i, size_t j) const {
      assert(matrix);
      return gsl_matrix_get(matrix, i, j);
    }

    inline GSLMatrix& Set(size_t i, size_t j, double value){
      assert(matrix);
      gsl_matrix_set(matrix, i, j, value);
      return *this;
    }
    inline GSLMatrix& SetAll(double value){
      assert(matrix);
      gsl_matrix_set_all(matrix, value);
      return *this;
    }
    inline void Zeros() { SetAll(0.0); }

    inline size_t Width() const {
      assert(matrix);
      return matrix->size2;
    }
    inline size_t Height() const {
      assert(matrix);
      return matrix->size1;
    }
    inline size_t Columns() const {return Width();}
    inline size_t Rows() const {return Height();}
    inline const gsl_matrix* GetPointer() const {
      assert(matrix);
      return matrix;
    }
    inline gsl_matrix* GetBarePointer() {
      assert(matrix);
      return matrix;
    }

    inline bool IsSquare() const {
      assert(matrix);
      return matrix->size1 == matrix->size2;
    }
};

#endif // GSLMATRIX_HH__0CCDE9F0_54AC_11E4_B002_283737241892

// GSLMatrix.hh ends here
