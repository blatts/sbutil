// -*- mode: C++ -*-
// Time-stamp: "2016-06-08 14:42:32 sb"

/*
  file       Form.hh
  copyright  (c) Sebastian Blatt 2015, 2016

  Everything in OutputManipulator.hh is written to take a particular
  type T as the first argument to the constructor, the idea being that
  we want to output T in a particular form. This is a bit limiting,
  because we cannot bind the OutputManipulator to another argument
  after initialization. Solve this by implementing another layer of
  redirection and inserting a base class BasicForm in between that
  handles a pointer internally that can be rebound after
  initialization.

  This file should become the basis of OutputManipulator version 2.

  Here is how to format a complicated STL type, like

    std::map<std::pair<uint32_t, uint8_t>, std::list<std::string>> data;
    data[std::make_pair(42, 1)] = {"Hello", "World"};

    form::number<uint32_t> fa;
    form::number<uint8_t> fb;
    form::identity<std::string> fc;

    auto fd = form::make_pair(fa, fb);
    auto fe = form::make_list(fc);
    auto ff = form::make_map(fd, fe);

    ff.bind(data);
    std::cout << ff << std::endl;

 */


#ifndef FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009
#define FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009

#include <iostream>
#include <iomanip>
#include <cassert>
#include <list>
#include <map>
#include <vector>

namespace form
{
  // Use RAII to implement unwind-protect on the ostream state to
  // make sure that the original stream state is restored on
  // destruction. Specifically, we wrap
  //
  //   std::ostream::flags
  //   std::ostream::fill
  //   std::ostream::precision
  //
  class ostream_unwind_protect {
    protected:
      std::ostream& out;
      std::ios_base::fmtflags flags;
      char fill;
      std::streamsize precision;
    public:
      ostream_unwind_protect(std::ostream& out_)
        : out(out_),
          flags(out_.flags()),
          fill(out_.fill()),
          precision(out_.precision())
      {}
      virtual ~ostream_unwind_protect(){
        if(out.good()){
          out.precision(precision);
          out.fill(fill);
          out.flags(flags);
        }
      }
  };

  // Base interface for a std::ostream manipulator that protects the
  // stream state using ostream_unwind_protect and provides operator<<
  // functionality. Derived classes need to define the output function
  // to emit something to std::ostream.
  //
  class manipulator {
    public:
      virtual std::ostream& output(std::ostream& out) const = 0;
      std::ostream& output_wrapper(std::ostream& out) const {
        if(out.good()){
          ostream_unwind_protect guard(out);
          output(out);
        }
        return out;
      }
  };
  inline std::ostream& operator<<(std::ostream& out, const manipulator& o) {
    return o.output_wrapper(out);
  }

  // Base interface for a manipulator that can be bound to an
  // arbitrary object of type T. Binding is done through const
  // pointers. If an instance is not bound to a pointer and output()
  // is called, the get_wrapped() function will assert(nullptr)
  // resulting in program termination.
  //
  // This is a feature because it indicates programmer brain damage.
  //
  template <typename T>
  class base : public manipulator {
    public:
      typedef T type;
    private:
      const T* pointer;
    protected:
      const T& get_wrapped() const {
        assert(pointer);
        return *pointer;
      }
    public:
      base() : pointer(nullptr) {}
      base(const T& t_) : pointer(&t_) {}
      base& bind(const T* pointer_) {pointer = pointer_; return *this;}
      base& bind(const T& t_) {pointer = &t_; return *this;}
  };

  // The most basic instantiable (non-interface) manipulator. Simply
  // pass the wrapped object to operator<<.
  //
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

  template <typename T>
  class number : public base<T> {
    public:
      number(){} // this automatically calls base()
      number(const T& t_) : base<T>(t_) {}
      std::ostream& output(std::ostream& out) const {
        const T& t = base<T>::get_wrapped();
        out << static_cast<long>(t);
        return out;
      }
  };

  template <typename T,
            typename item_form = identity<typename T::value_type>
            >
  class list : public base<T> {
    private:
      item_form& f;
    public:
      list(item_form& f_)
        : base<T>(), f(f_)
      {}
      list(const T& t_, item_form& f_)
        : base<T>(t_), f(f_)
      {}
      std::ostream& output(std::ostream& out) const {
        const T& t = base<T>::get_wrapped();
        out << "( ";
        for(auto cit = t.begin(); cit != t.end(); ++cit){
          out << f.bind(*cit) << ", ";
        }
        out << ")";
        return out;
      }
  };

  // Convenience constructor wrappers so we do not have to specify the
  // lengthy templated types for normal use. Modelled after
  // std::make_pair.

  // unbound version
  template<typename item_form>
  list<std::list<typename item_form::type>, item_form>
  make_list(item_form& f_) {
    return list<std::list<typename item_form::type>, item_form> (f_);
  }

  // bound version
  template<typename item_form>
  list<std::list<typename item_form::type>, item_form>
  make_list(std::list<typename item_form::type>& x_, item_form& f_) {
    return list<std::list<typename item_form::type>, item_form> (x_, f_);
  }


  template <typename T,
            typename first_form = identity<typename T::first_type>,
            typename second_form = identity<typename T::second_type>
            >
  class pair : public base<T> {
    private:
      first_form& ff;
      second_form& sf;
    public:
      pair(first_form& ff_, second_form& sf_)
        : base<T>(), ff(ff_), sf(sf_)
      {}
      pair(const T& t_, first_form& ff_, second_form& sf_)
        : base<T>(t_), ff(ff_), sf(sf_)
      {}
      std::ostream& output(std::ostream& out) const {
        const T& t = base<T>::get_wrapped();
        out << "("
            << ff.bind(t.first) << " . "
            << sf.bind(t.second)
            << ")";
        return out;
      }
  };

  // Convenience function so we do not have to specify the lengthy
  // templated types for normal use. Modelled after std::make_pair.
  template<typename first_form, typename second_form>
  pair<std::pair<typename first_form::type, typename second_form::type>,
       first_form, second_form> make_pair(first_form& ff_, second_form& sf_)
  {
    return pair<std::pair<typename first_form::type, typename second_form::type>,
                first_form, second_form> (ff_, sf_);
  }



  template <typename T,
            typename key_form = identity<typename T::key_type>,
            typename value_form = identity<typename T::mapped_type>
            >
  class map : public base<T> {
    private:
      key_form& kf;
      value_form& vf;
    public:
      map(key_form& kf_,
                value_form& vf_)
        : base<T>(),
          kf(kf_),
          vf(vf_)
      {}
      map(const T& t_,
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

  // Convenience function so we do not have to specify the lengthy
  // templated types for normal use. Modelled after std::make_pair.
  template<typename key_form, typename value_form>
  map<std::map<typename key_form::type,
                typename value_form::type>,
       key_form, value_form>
  make_map(key_form& kf_, value_form& vf_)
  {
    return map<std::map<typename key_form::type,
                        typename value_form::type>,
                key_form,
                value_form> (kf_, vf_);
  }


}


#endif // FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009

// Form.hh ends here
