// -*- mode: C++ -*-
// Time-stamp: "2014-07-03 15:30:33 sb"

/*
  file       QuantumState.hh
  copyright  (c) Sebastian Blatt 2014

 */


#ifndef QUANTUMSTATE_HH__EC5D7F7C_EAA3_11E3_A6E6_283737241892
#define QUANTUMSTATE_HH__EC5D7F7C_EAA3_11E3_A6E6_283737241892

#include <sbutil/Representable.hh>
#include <sbutil/Exception.hh>

#include <complex>
#include <vector>
#include <map>


template<typename A, typename B>
std::ostream& operator<<(std::ostream& out, const std::pair<A, B>& X){
  out << X.first << ", " << X.second;
  return out;
}


template<typename QuantumNumber>
class Basis : public Representable {
  public:
    typedef QuantumNumber quantum_number_t;

  protected:
    size_t rank;
    std::vector<QuantumNumber> index_to_quantum_number;

  public:
    Basis(size_t rank_ = 0)
      : rank(rank_),
        index_to_quantum_number(rank_)
    {}

    size_t Rank() const {return rank;}

    QuantumNumber QuantumNumberFromIndex(size_t index) const {
      if(index >= rank){
        std::ostringstream os;
        os << "Basis index " << index << " larger than rank-1 = "
           << rank - 1;
        throw EXCEPTION(os.str());
      }
      return index_to_quantum_number[index];
    }


    std::ostream& Represent(std::ostream& out) const {
      for(size_t i=0; i<rank; ++i){
        out << i << " -> " << index_to_quantum_number[i];
        if(i < rank - 1){
          out << "\n";
        }
      }
      return out;
    }
};

template<typename BasisA,
         typename BasisB
         >
class ProductBasis :
  public Basis<std::pair< typename BasisA::quantum_number_t,
                          typename BasisB::quantum_number_t>
               >
{
  public:
    typedef std::pair<typename BasisA::quantum_number_t,
                      typename BasisB::quantum_number_t> quantum_number_t;

  private:
    const BasisA& A;
    const BasisB& B;
  public:
    ProductBasis(const BasisA& A_,
                 const BasisB& B_)
      : A(A_),
        B(B_)
    {
      size_t rank_A = A.Rank();
      size_t rank_B = B.Rank();
      this->rank = rank_A * rank_B;

      this->index_to_quantum_number.resize(this->rank);
      for(size_t i=0; i<rank_A; ++i){
        for(size_t j=0; j<rank_B; ++j){

          this->index_to_quantum_number[i*rank_B + j] = quantum_number_t(A.QuantumNumberFromIndex(i),
                                                                         B.QuantumNumberFromIndex(j));
        }
      }
    }
};

#define TENSOR_PRODUCT2_T(A, B) ProductBasis< decltype(A), decltype(B) >
#define TENSOR_PRODUCT2(A, B) TENSOR_PRODUCT2_T(A, B)(A, B)
#define TENSOR_PRODUCT3_T(A, B, C) ProductBasis< TENSOR_PRODUCT2_T(A, B), decltype(C) >
#define TENSOR_PRODUCT3(A, B, C) TENSOR_PRODUCT3_T(A, B, C)(TENSOR_PRODUCT2(A, B), C)


typedef double HalfInteger;
class SpinBasis : public Basis<HalfInteger> {
  public:
    SpinBasis(HalfInteger spin_modulus)
      : Basis(2 * spin_modulus + 1)
    {
      for(size_t i=0; i<this->rank; ++i){
        this->index_to_quantum_number[i] = -spin_modulus + i;
      }
    }
};

class HarmonicOscillatorBasis : public Basis<unsigned int> {
  public:
    HarmonicOscillatorBasis(unsigned int maximal_quantum_number)
      : Basis(maximal_quantum_number + 1)
    {
      for(size_t i=0; i<this->rank; ++i){
        this->index_to_quantum_number[i] = i;
      }
    }
};



#endif // QUANTUMSTATE_HH__EC5D7F7C_EAA3_11E3_A6E6_283737241892

// QuantumState.hh ends here
