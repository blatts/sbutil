// -*- mode: C++ -*-
// Time-stamp: "2016-02-08 16:34:51 sb"

/*
  file       Sparse.hh
  copyright  (c) Sebastian Blatt 2015, 2016

  FIXME: Multiplication on StorageFlat and StorageColumnThenRow are broken.

*/


#ifndef SPARSE_HH__C3BA39E2_C47E_11E4_8EF0_283737241892
#define SPARSE_HH__C3BA39E2_C47E_11E4_8EF0_283737241892

#include <map>
#include <unordered_map>
#include <sstream>
#include <vector>

#include <sbutil/Exception.hh>
#include <sbutil/Representable.hh>
#include <sbutil/GSLMatrix.hh>

namespace Sparse {

  template<typename T>
  class Storage {
    protected:
      const size_t n_rows;
      const size_t n_columns;
      const T default_value;

    public:
      Storage(size_t n_rows_, size_t n_columns_,
                          T default_value_)
        : n_rows(n_rows_),
          n_columns(n_columns_),
          default_value(default_value_)
      {}

      size_t Rows() const {return n_rows;}
      size_t Columns() const {return n_columns;}
      const T DefaultValue() const {return default_value;}

      void ValidateIndex(size_t i, size_t j) const {
        if(i >= Storage<T>::n_rows || j >= Storage<T>::n_columns){
          std::ostringstream os;
          os << "index (" << i << ", " << j << ") not in [(0, 0), ("
             << Storage<T>::n_rows-1 << ", " << Storage<T>::n_columns-1 << ")]";
          throw EXCEPTION(os.str());
        }
      }

      virtual const T Access(size_t i, size_t j) const = 0;

      const T AccessChecked(size_t i, size_t j) const {
        ValidateIndex(i, j);
        return Access(i, j);
      }

      virtual void Set(size_t i, size_t j, T t) = 0;

      void SetChecked(size_t i, size_t j, T t) {
        ValidateIndex(i, j);
        Set(i, j, t);
      }

      virtual void MultiplyByColumnVector(const T* column,
                                          T* result) const = 0;

      virtual GSLMatrix* ToDense() const = 0;
  };

  template<typename T>
  class StorageFlat : public Storage<T> {
    private:
      typedef size_t key_t;
      typedef std::map<key_t, T> data_t;
      // seems to be about twice as slow, maybe because we already prehash
      // manually?
      //typedef std::unordered_map<key_t, T> data_t;
      typedef typename data_t::iterator data_it;
      typedef typename data_t::const_iterator data_cit;
      data_t data;

    public:
      StorageFlat(size_t n_rows_, size_t n_columns_,
                  T default_value_)
        : Storage<T>(n_rows_, n_columns_, default_value_),
          data()
      {}

      inline key_t Index(size_t i, size_t j) const {
        return i * Storage<T>::n_columns + j;
      }

      const T Access(size_t i, size_t j) const {
        key_t k = Index(i, j);
        data_cit it = data.find(k);
        if(it != data.end()) {
          return it->second;
        }
        return Storage<T>::default_value;
      }

      void Set(size_t i, size_t j, T t) {
        key_t k = Index(i, j);
        data[k] = t;
      }

      void MultiplyByColumnVector(const T* column,
                                  T* result) const
      {
        for(size_t i=0; i<Storage<T>::n_rows; ++i){
          double rc = 0.0;
          for(size_t j=0; j<Storage<T>::n_columns; ++j){
            rc += Access(i, j) * column[j];
          }
          result[i] = rc;
        }
      }

      GSLMatrix* ToDense() const{
        GSLMatrix* p = new GSLMatrix(Storage<T>::n_rows,
                                     Storage<T>::n_columns,
                                     Storage<T>::default_value);
        for(data_cit it = data.begin(); it != data.end(); ++it){
          size_t j = it->first % Storage<T>::n_columns;
          size_t i = it->first / Storage<T>::n_columns;
          p->Set(i, j, it->second);
        }
        return p;
      }

  };

  template<typename T>
  class StorageRowThenColumn : public Storage<T> {
    private:
      typedef size_t key_t;
      typedef std::map<key_t, T> row_data_t;
      typedef typename row_data_t::iterator row_data_it;
      typedef typename row_data_t::const_iterator row_data_cit;

      typedef std::map<key_t, row_data_t> data_t;
      typedef typename data_t::iterator data_it;
      typedef typename data_t::const_iterator data_cit;

      data_t data;

    public:
      StorageRowThenColumn(size_t n_rows_, size_t n_columns_,
                  T default_value_)
        : Storage<T>(n_rows_, n_columns_, default_value_),
        data()
      {}

      const T Access(size_t i, size_t j) const {
        data_cit it = data.find(i);
        if(it != data.end()) {
          const row_data_t& row = it->second;
          row_data_cit itr = row.find(j);
          if(itr != row.end()){
            return itr->second;
          }
        }
        return Storage<T>::default_value;
      }

      void Set(size_t i, size_t j, T t) {
        data_it it = data.find(i);
        if(it == data.end()){
          data[j] = row_data_t();
        }
        row_data_t& row = data[i];
        row[j] = t;
      }

      void MultiplyByColumnVector(const T* column,
                                  T* result) const
      {
        for(data_cit cit = data.begin(); cit != data.end(); ++cit){
          //const size_t i = cit->first;
          const row_data_t& row = cit->second;
          T rc = 0;
          for(row_data_cit cjt = row.begin(); cjt != row.end(); ++cjt){
            //const size_t j = cjt->first;
            rc += column[cjt->first] * cjt->second;
          }
          result[cit->first] = rc;
        }
      }

      GSLMatrix* ToDense() const{
        GSLMatrix* p = new GSLMatrix(Storage<T>::n_rows,
                                     Storage<T>::n_columns,
                                     Storage<T>::default_value);
        for(data_cit cit = data.begin(); cit != data.end(); ++cit){
          const row_data_t& row = cit->second;
          for(row_data_cit cjt = row.begin(); cjt != row.end(); ++cjt){
            p->Set(cit->first, cjt->first, cjt->second);
          }
        }
        return p;
      }

  };



  template<typename T>
  class StorageColumnThenRow : public Storage<T> {
    private:
      typedef size_t key_t;
      typedef std::map<key_t, T> col_data_t;
      typedef typename col_data_t::iterator col_data_it;
      typedef typename col_data_t::const_iterator col_data_cit;

      typedef std::map<key_t, col_data_t> data_t;
      typedef typename data_t::iterator data_it;
      typedef typename data_t::const_iterator data_cit;

      data_t data;

    public:
      StorageColumnThenRow(size_t n_rows_, size_t n_columns_,
                           T default_value_)
        : Storage<T>(n_rows_, n_columns_, default_value_),
        data()
      {}

      const T Access(size_t i, size_t j) const {
        data_cit it = data.find(j);
        if(it != data.end()) {
          const col_data_t& col = it->second;
          col_data_cit itr = col.find(i);
          if(itr != col.end()){
            return itr->second;
          }
        }
        return Storage<T>::default_value;
      }

      void Set(size_t i, size_t j, T t) {
        data_it it = data.find(j);
        if(it == data.end()){
          data[j] = col_data_t();
        }
        col_data_t& col = data[j];
        col[i] = t;
      }

      void MultiplyByColumnVector(const T* column,
                                  T* result) const
      {
        for(data_cit cjt = data.begin(); cjt != data.end(); ++cjt){
          //const size_t j = cjt->first; // column index
          const col_data_t& col = cjt->second;
          for(col_data_cit cit = col.begin(); cit != col.end(); ++cit){
            //const size_t i = cit->first; // row index
            result[cit->first] += column[cjt->first] * cit->second;
          }
        }
      }

      GSLMatrix* ToDense() const{
        GSLMatrix* p = new GSLMatrix(Storage<T>::n_rows,
                                     Storage<T>::n_columns,
                                     Storage<T>::default_value);
        for(data_cit cjt = data.begin(); cjt != data.end(); ++cjt){
          const col_data_t& col = cjt->second;
          for(col_data_cit cit = col.begin(); cjt != col.end(); ++cjt){
            p->Set(cit->first, cjt->first, cit->second);
          }
        }
        return p;
      }

  };



  template<typename T, typename S>
  class Matrix : public Representable {
    private:
      S storage;

    public:
      Matrix(size_t n_rows_, size_t n_columns_,
                   T default_value_ = 0);
      ~Matrix();

      inline const T& operator()(size_t i, size_t j) const {
        return storage.AccessChecked(i, j);
      }

      inline const T Get(size_t i, size_t j) const{
        return storage.AccessChecked(i, j);
      }

      inline const T GetUnsafe(size_t i, size_t j) const {
        return storage.Access(i, j);
      }

      inline Matrix& Set(size_t i, size_t j, const T& t){
        storage.SetChecked(i, j, t);
        return *this;
      }

      inline Matrix& SetUnsafe(size_t i, size_t j, const T& t){
        storage.Set(i, j, t);
        return *this;
      }
      // FIXME: this could be done better at the Storage level when
      // specializing for each child class.
      inline Matrix& Add(size_t i, size_t j, T t){
        double x = Get(i, j);
        Set(i, j, x + t);
        return *this;
      }


      inline size_t Columns() const {return storage.Columns();}
      inline size_t Rows() const {return storage.Rows();}


      void MultiplyByColumnVector(const std::vector<T>& column,
                                  std::vector<T>& result) const;
      void MultiplyByColumnVector(const T* column,
                                  size_t N_column,
                                  T* result,
                                  size_t N_result) const;

      GSLMatrix* ToDense() const {return storage.ToDense();}

      std::ostream& Represent(std::ostream& out) const;

    private:
      const Matrix& operator=(const Matrix&){}
  };

  template<typename T, typename S>
  Matrix<T, S>::Matrix(size_t n_rows_, size_t n_columns_,
                                T default_value_)
    : storage(n_rows_, n_columns_, default_value_)
  {}

  template<typename T, typename S>
  Matrix<T, S>::~Matrix(){}

  template<typename T, typename S>
  void Matrix<T, S>::MultiplyByColumnVector(const std::vector<T>& column,
                                            std::vector<T>& result) const
  {
    const size_t c = storage.Columns();
    const size_t r = storage.Rows();

    if(column.size() != c){
      std::ostringstream os;
      os << "Vector has " << column.size() << " rows, matrix has "
         << c << " columns";
      throw EXCEPTION(os.str());
    }

    result.resize(r);
    std::fill(result.begin(), result.end(), 0.0);
    storage.MultiplyByColumnVector(&(column[0]), &(result[0]));
  }

  template<typename T, typename S>
  void Matrix<T, S>::MultiplyByColumnVector(const T* column,
                                            size_t N_column,
                                            T* result,
                                            size_t N_result) const
  {
    const size_t c = storage.Columns();
    const size_t r = storage.Rows();

    if(N_column != c){
      std::ostringstream os;
      os << "Contracting vector has " << N_column
         << " rows, matrix has " << c << " columns";
      throw EXCEPTION(os.str());
    }
    if(N_result != r){
      std::ostringstream os;
      os << "Result vector has " << N_result << " rows, matrix has "
         << r << " rows";
      throw EXCEPTION(os.str());
    }
    memset(result, 0, sizeof(T)*N_result);
    storage.MultiplyByColumnVector(column, result);
  }

  template<typename T, typename S>
  std::ostream& Matrix<T, S>::Represent(std::ostream& out) const {
    out << "Matrix with " << storage.Rows() << " rows, "
        << storage.Columns() << " columns\n"
        << "default value = " << storage.DefaultValue() << "\n";

    // for(data_cit i=data.begin(); i!=data.end(); ++i){
    //   std::pair<size_t, size_t> ij = storage.IndexInverse(i->first);
    //   out << "  (" << ij.first << ", " << ij.second << ") : "
    //       << i->second << "\n";
    // }
    return out;
  }

  //typedef Matrix<double, StorageFlat<double> > SparseMatrix;
  typedef Matrix<double, StorageRowThenColumn<double> > SparseMatrix;
  //typedef Matrix<double, StorageColumnThenRow<double> > SparseMatrix;
}

#endif // SPARSE_HH__C3BA39E2_C47E_11E4_8EF0_283737241892

// Sparse.hh ends here
