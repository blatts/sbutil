// -*- mode: C++ -*-
// Time-stamp: "2014-11-13 09:44:44 sb"

/*
  file       VectorTools.hh
  copyright  (c) Sebastian Blatt 2014

  Convenience functions to work with std::vector<T> with numeric types
  T. Use this mostly as a shallow convenience wrapper around standard
  library functions.

  Could have used functors and std::generate, but simple loops seem
  more transparent in some cases.

 */


#ifndef VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892
#define VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892

#include <vector>
#include <cmath>
#include <numeric>
#include <functional>

template <typename T>
T sqr(const T& t){
  return t * t;
}

template <typename T>
void linspace(const T a, const T b, const size_t N, std::vector<T>& result){
  result.resize(N);
  const T delta = (b - a) / static_cast<T>(N-1);
  result[0] = a;
  for(size_t i=1; i<N; ++i){
    result[i] = result[i-1] + delta;
  }
}

template <typename T>
void range(const T a, const T b, const T delta, std::vector<T>& result){
  const size_t N = std::floor((b-a) / delta);
  result.resize(N);
  result[0] = a;
  for(size_t i=1; i<N; ++i){
    result[i] = result[i-1] + delta;
  }
}

template <typename T>
T total(const std::vector<T>& v){
  return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

template <typename T>
T product(const std::vector<T>& v){
  return std::accumulate(v.begin(), v.end(),
                         static_cast<T>(1), std::multiplies<T>());
}

template <typename T>
T dot(const std::vector<T>& v, const std::vector<T>& w){
  return std::inner_product(v.begin(), v.end(), w.begin(),
                            static_cast<T>(0));
}


template <typename T>
const std::vector<T>& scale(std::vector<T>& v, const T scale){
  const size_t N = v.size();
  for(size_t i=0; i<N; ++i){
    v[i] *= scale;
  }
  return v;
}

template <typename T>
void normalize_by_total(std::vector<T>& v){
  const T t = 1.0 / total(v);
  scale(v, t);
}

template <typename T>
void normalize(std::vector<T>& v){
  T t = dot(v, v);
  scale(v, 1.0 / sqrt(t));
}

template <typename T>
inline const std::vector<T>& appendto(std::vector<T>& v, const std::vector<T>& w){
  v.reserve(v.size() + w.size());
  std::copy(w.begin(), w.end(), std::back_insert_iterator< std::vector<T> >(v));
  return v;
}




#endif // VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892

// VectorTools.hh ends here
