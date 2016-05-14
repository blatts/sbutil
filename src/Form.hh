// -*- mode: C++ -*-
// Time-stamp: "2015-03-15 08:13:48 sb"

/*
  file       Form.hh
  copyright  (c) Sebastian Blatt 2015

  Everything in OutputManipulator.hhq is written to take a particular
  type T as the first argument to the constructor, the idea being that
  we want to output T in a particular form. This is a bit limiting,
  because we cannot bind the OutputManipulator to another argument
  after initialization. Solve this by implementing another layer of
  redirection and inserting a base class BasicForm in between that
  handles a pointer internally that can be rebound after
  initialization.

  This file should become the basis of OutputManipulator version 2.
 */


#ifndef FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009
#define FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009

#include "OutputManipulator.hh"
#include "Exception.hh"

namespace form
{

  template <typename T>
  class base : public OutputManipulator {
    public:
      typedef T type;
    private:
      const T* pointer;
    protected:
      const T& get_wrapped() const {
        if(!pointer){
          std::ostringstream os;
          os << "base<" << typeid(type).name() << ">::get_wrapped(): "
             << "not bound (pointer == nullptr)";
          throw EXCEPTION(os.str());
        }
        return *pointer;
      }
    public:
      base() : pointer(nullptr) {}
      base(const T& t_) : pointer(&t_) {}
      base& bind(const T* pointer_) {pointer = pointer_; return *this;}
      base& bind(const T& t_) {pointer = &t_; return *this;}
  };

  template <typename T>
  class identity : public base<T> {
    public:
      // C++11 trick:
      //identity(){} // this automatically calls base()
      //identity(const T& t_) : base<T>(t_) {}
      using base<T>::base;

      std::ostream& output(std::ostream& out) const {
        out << base<T>::get_wrapped();
        return out;
      }
  };

  template <typename T,
            typename key_form = identity<typename T::key_type>,
            typename value_form = identity<typename T::mapped_type>
            >
  class dict_form : public base<T> {
    private:
      key_form& kf;
      value_form& vf;
    public:
      dict_form(key_form& kf_,
                value_form& vf_)
        : base<T>(),
          kf(kf_),
          vf(vf_)
      {}
      dict_form(const T& t_,
                key_form& kf_,
                value_form& vf_)
        : base<T>(t_),
          kf(kf_),
          vf(vf_)
      {}
      std::ostream& output(std::ostream& out) const {
        const T& t = base<T>::get_wrapped();
        for(auto cit = t.begin(); cit != t.end(); ++cit){
          out << kf.bind(cit->first) << "\t"
              << vf.bind(cit->second) << "\n";
        }
        return out;
      }

  };

}


#endif // FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009

// Form.hh ends here
