// -*- mode: C++ -*-
// Time-stamp: "2015-08-18 18:54:19 sb"

/*
  file       Superposition.hh
  copyright  (c) Sebastian Blatt 2015

 */


#ifndef SUPERPOSITION_HH__896367C0_4505_11E5_8A1E_283737241892
#define SUPERPOSITION_HH__896367C0_4505_11E5_8A1E_283737241892

#include <sbutil/Representable.hh>

#include <vector>
#include <memory>
#include <type_traits>

template<typename S>
class Superposition : public Representable {
  public:
    typedef std::shared_ptr<S> ptr_t;
    typedef std::vector<ptr_t> vector_t;
  protected:
    vector_t elements;

  public:
    Superposition()
      : elements()
    {}
    virtual ~Superposition(){}

    // see https://stackoverflow.com/questions/32054116/c11-variadic-template-inheritance
    template<typename... Parameters,
             typename Result = typename std::result_of<S&(Parameters const&...)>::type
             >
    Result operator()(const Parameters&... parameters) const {
      Result result = {};
      for(auto p : elements){
        result += (*p)(parameters...);
      }
      return result;
    }

    void Add(S* p){
      elements.push_back(ptr_t(p));
    }
    void Clear(){
      elements.clear();
    }
    size_t Size() const {
      return elements.size();
    }
    const S& At(size_t i) const{
      return *elements[i];
    }

    std::ostream& Represent(std::ostream& out) const {
      out << ptr_vector_form<S, std::shared_ptr<S> >(elements);
      return out;
    }

};


#endif // SUPERPOSITION_HH__896367C0_4505_11E5_8A1E_283737241892

// Superposition.hh ends here
