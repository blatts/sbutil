// -*- mode: C++ -*-
// Time-stamp: "2014-11-13 11:59:01 sb"

/*
  file       VectorTools.hh
  copyright  (c) Sebastian Blatt 2014

  Convenience functions to work with std::vector<T> with numeric types
  T. Use this mostly as a shallow convenience wrapper around standard
  library functions. Main use of most of these is to avoid having to
  write v.begin(), v.end() all over the place and provide Matlab-style
  names.

  Could have used functors and std::generate, but simple loops seem
  more transparent in some cases.

 */


#ifndef VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892
#define VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892

#include <vector>
#include <cmath>
#include <numeric>
#include <functional>
#include <algorithm>

template <typename T>
inline T sqr(const T& t){
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
inline T total(const std::vector<T>& v){
  return std::accumulate(v.begin(), v.end(), static_cast<T>(0));
}

template <typename T>
inline T product(const std::vector<T>& v){
  return std::accumulate(v.begin(), v.end(),
                         static_cast<T>(1), std::multiplies<T>());
}

template <typename T>
inline T dot(const std::vector<T>& v, const std::vector<T>& w){
  return std::inner_product(v.begin(), v.end(), w.begin(),
                            static_cast<T>(0));
}

template <typename T, typename UnaryOperation>
inline const std::vector<T>& apply(std::vector<T>& v, UnaryOperation op){
  std::transform(v.begin(), v.end(), v.begin(), op);
  return v;
}

template <typename T>
inline const std::vector<T>& scale(std::vector<T>& v, const T scale){
  // Avoid using std::transform, std::bind2nd and std::multiplies functors
  // for such a simple operation.
  const size_t N = v.size();
  for(size_t i=0; i<N; ++i){
    v[i] *= scale;
  }
  return v;
}

template <typename T>
inline void normalize_by_total(std::vector<T>& v){
  const T t = 1.0 / total(v);
  scale(v, t);
}

template <typename T>
inline void normalize(std::vector<T>& v){
  T t = dot(v, v);
  scale(v, 1.0 / sqrt(t));
}

template <typename T>
inline const std::vector<T>& appendto(std::vector<T>& v, const std::vector<T>& w){
  v.reserve(v.size() + w.size());
  std::copy(w.begin(), w.end(), std::back_insert_iterator< std::vector<T> >(v));
  return v;
}

// Copy full vector<A> <- vector<B> using default casting between A <- B
template <typename A, typename B>
inline std::vector<A>& copy(std::vector<A>& dst, const std::vector<B>& src){
  dst.resize(src.size());
  std::copy(src.begin(), src.end(), dst.begin());
  return dst;
}


#endif // VECTORTOOLS_HH__F8B8D66C_EA68_11E3_8491_283737241892

// VectorTools.hh ends here
