// -*- mode: C++ -*-
// Time-stamp: "2018-03-15 18:00:01 srlab"

/*
  file       Form.hh
  copyright  (c) Sebastian Blatt 2015, 2016, 2017, 2018

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

  std::cout << ff(data) << std::endl;

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
        base& operator()(const T* pointer_) {return bind(pointer_);}
        base& operator()(const T& t_) {return bind(t_);}
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


// ------------------------------------------------------------------------- list

 #define FORM_LIST_DEFAULT_SEPARATOR ", "
 #define FORM_LIST_DEFAULT_OPEN "( "
 #define FORM_LIST_DEFAULT_CLOSE " )"

    template <typename T,
              typename item_form = identity<typename T::value_type>
              >
    class list : public base<T> {
      private:
        item_form& f;
        const char* separator;
        const char* open;
        const char* close;

      public:
        list(item_form& f_,
             const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR,
             const char* open_ = FORM_LIST_DEFAULT_OPEN,
             const char* close_ = FORM_LIST_DEFAULT_CLOSE
            )
            : base<T>(),
            f(f_),
            separator(separator_),
            open(open_),
            close(close_)
        {}
        list(const T& t_,
             item_form& f_,
             const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR,
             const char* open_ = FORM_LIST_DEFAULT_OPEN,
             const char* close_ = FORM_LIST_DEFAULT_CLOSE
            )
            : base<T>(t_),
            f(f_),
            separator(separator_),
            open(open_),
            close(close_)
        {}
        std::ostream& output(std::ostream& out) const {
            const T& t = base<T>::get_wrapped();
            out << open;
            for(auto cit = t.begin(); cit != t.end(); ++cit){
                out << f.bind(*cit);
                if(cit + 1 != t.end()) {
                    out << separator;
                }
            }
            out << close;
            return out;
        }
    };

// Convenience constructor wrappers so we do not have to specify the
// lengthy templated types for normal use. Modelled after
// std::make_pair.

// unbound version
    template<typename item_form>
    list<std::list<typename item_form::type>, item_form>
    make_list(item_form& f_,
              const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR,
              const char* open_ = FORM_LIST_DEFAULT_OPEN,
              const char* close_ = FORM_LIST_DEFAULT_CLOSE
             ) {
        return list<std::list<typename item_form::type>, item_form>
        (f_, separator_, open_, close_);
    }

// bound version
    template<typename item_form>
    list<std::list<typename item_form::type>, item_form>
    make_list(std::list<typename item_form::type>& x_,
              item_form& f_,
              const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR,
              const char* open_ = FORM_LIST_DEFAULT_OPEN,
              const char* close_ = FORM_LIST_DEFAULT_CLOSE
             )
    {
        return list<std::list<typename item_form::type>, item_form>
        (x_, f_, separator_, open_, close_);
    }

// ----------------------------------------------------------------------- vector

#define FORM_VECTOR_DEFAULT_SEPARATOR ", "
#define FORM_VECTOR_DEFAULT_OPEN "[ "
#define FORM_VECTOR_DEFAULT_CLOSE " ]"

    template <typename T,
              typename item_form = identity<typename T::value_type>
              >
    class vector : public base<T> {
      private:
        item_form& f;
        const char* separator;
        const char* open;
        const char* close;

      public:
        vector(item_form& f_,
               const char* separator_ = FORM_VECTOR_DEFAULT_SEPARATOR,
               const char* open_ = FORM_VECTOR_DEFAULT_OPEN,
               const char* close_ = FORM_VECTOR_DEFAULT_CLOSE
              )
            : base<T>(),
            f(f_),
            separator(separator_),
            open(open_),
            close(close_)
        {}

        vector(const T& t_,
               item_form& f_,
               const char* separator_ = FORM_VECTOR_DEFAULT_SEPARATOR,
               const char* open_ = FORM_VECTOR_DEFAULT_OPEN,
               const char* close_ = FORM_VECTOR_DEFAULT_CLOSE
              )
            : base<T>(t_),
            f(f_),
            separator(separator_),
            open(open_),
            close(close_)
        {}

        std::ostream& output(std::ostream& out) const {
            const T& t = base<T>::get_wrapped();
            out << open;
            for(auto cit = t.begin(); cit != t.end(); ++cit){
                out << f.bind(*cit);
                if(cit + 1 != t.end()) {
                    out << separator;
                }
            }
            out << close;
            return out;
        }
    };

// Convenience constructor wrappers so we do not have to specify the
// lengthy templated types for normal use. Modelled after
// std::make_pair.

// unbound version
    template<typename item_form>
    vector<std::vector<typename item_form::type>, item_form>
    make_vector(item_form& f_,
                const char* separator_ = FORM_VECTOR_DEFAULT_SEPARATOR,
                const char* open_ = FORM_VECTOR_DEFAULT_OPEN,
                const char* close_ = FORM_VECTOR_DEFAULT_CLOSE
               )
    {
        return vector<std::vector<typename item_form::type>, item_form>
        (f_, separator_, open_, close_);
    }

// bound version
    template<typename item_form>
    vector<std::vector<typename item_form::type>, item_form>
    make_vector(std::vector<typename item_form::type>& x_,
                item_form& f_,
                const char* separator_ = FORM_VECTOR_DEFAULT_SEPARATOR,
                const char* open_ = FORM_VECTOR_DEFAULT_OPEN,
                const char* close_ = FORM_VECTOR_DEFAULT_CLOSE
               )
    {
        return vector<std::vector<typename item_form::type>, item_form>
        (x_, f_, separator_, open_, close_);
    }


// ------------------------------------------------------------------------- pair

#define FORM_PAIR_DEFAULT_SEPARATOR " . "
#define FORM_PAIR_DEFAULT_OPEN "("
#define FORM_PAIR_DEFAULT_CLOSE ")"

    template <typename T,
              typename first_form = identity<typename T::first_type>,
              typename second_form = identity<typename T::second_type>
              >
    class pair : public base<T> {
      private:
        first_form& ff;
        second_form& sf;
        const char* open;
        const char* close;
        const char* separator;

      public:
        pair(first_form& ff_,
             second_form& sf_,
             const char* open_ = FORM_LIST_DEFAULT_OPEN,
             const char* close_ = FORM_LIST_DEFAULT_CLOSE,
             const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR
            )
            : base<T>(),
            ff(ff_),
            sf(sf_),
            open(open_),
            close(close_),
            separator(separator_)
        {}
        pair(const T& t_,
             first_form& ff_,
             second_form& sf_,
             const char* open_ = FORM_LIST_DEFAULT_OPEN,
             const char* close_ = FORM_LIST_DEFAULT_CLOSE,
             const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR
            )
            : base<T>(t_),
            ff(ff_),
            sf(sf_),
            open(open_),
            close(close_),
            separator(separator_)
        {}
        std::ostream& output(std::ostream& out) const {
            const T& t = base<T>::get_wrapped();
            out << open
                << ff.bind(t.first) << separator
                << sf.bind(t.second)
                << close;
            return out;
        }
    };

// Convenience function so we do not have to specify the lengthy
// templated types for normal use. Modelled after std::make_pair.
    template<typename first_form, typename second_form>
    pair<std::pair<typename first_form::type, typename second_form::type>,
         first_form, second_form> make_pair(first_form& ff_,
                                            second_form& sf_,
                                            const char* open_ = FORM_LIST_DEFAULT_OPEN,
                                            const char* close_ = FORM_LIST_DEFAULT_CLOSE,
                                            const char* separator_ = FORM_LIST_DEFAULT_SEPARATOR)
    {
        return pair<std::pair<typename first_form::type, typename second_form::type>,
                    first_form, second_form> (ff_, sf_, open_, close_, separator_);
    }


// -------------------------------------------------------------------------- map

#define FORM_MAP_DEFAULT_OPEN "{\n"
#define FORM_MAP_DEFAULT_CLOSE "\n}"
#define FORM_MAP_DEFAULT_SEPARATOR ",\n"
#define FORM_MAP_DEFAULT_MAPSTO " : "

    template <typename T,
              typename key_form = identity<typename T::key_type>,
              typename value_form = identity<typename T::mapped_type>
              >
    class map : public base<T> {
      private:
        key_form& kf;
        value_form& vf;
        const char* open;
        const char* close;
        const char* separator;
        const char* mapsto;

      public:
        map(key_form& kf_,
            value_form& vf_,
            const char* open_ = FORM_MAP_DEFAULT_OPEN,
            const char* close_ = FORM_MAP_DEFAULT_CLOSE,
            const char* separator_ = FORM_MAP_DEFAULT_SEPARATOR,
            const char* mapsto_ = FORM_MAP_DEFAULT_MAPSTO
           )
            : base<T>(),
            kf(kf_),
            vf(vf_),
            open(open_),
            close(close_),
            separator(separator_),
            mapsto(mapsto_)
        {}
        map(const T& t_,
            key_form& kf_,
            value_form& vf_,
            const char* open_ = FORM_MAP_DEFAULT_OPEN,
            const char* close_ = FORM_MAP_DEFAULT_CLOSE,
            const char* separator_ = FORM_MAP_DEFAULT_SEPARATOR,
            const char* mapsto_ = FORM_MAP_DEFAULT_MAPSTO
           )
            : base<T>(t_),
            kf(kf_),
            vf(vf_),
            open(open_),
            close(close_),
            separator(separator_),
            mapsto(mapsto_)
        {}
        std::ostream& output(std::ostream& out) const {
            const T& t = base<T>::get_wrapped();
            out << open;
            for(auto cit = t.begin(); cit != t.end(); ++cit){
                out << kf.bind(cit->first)
                    << mapsto
                    << vf.bind(cit->second);
                if(cit + 1 != t.end()){
                    out << separator;
                }
            }
            out << close;
            return out;
        }

    };

// Convenience function so we do not have to specify the lengthy
// templated types for normal use. Modelled after std::make_pair.
    template<typename key_form, typename value_form>
    map<std::map<typename key_form::type,
                 typename value_form::type>,
        key_form, value_form>
    make_map(key_form& kf_,
             value_form& vf_,
             const char* open_ = "{\n",
             const char* close_ = "\n}",
             const char* separator_ = ",\n",
             const char* mapsto_ = " : "
            )
    {
        return map<std::map<typename key_form::type,
                            typename value_form::type>,
                   key_form,
                   value_form> (kf_, vf_, open_, close_, separator_, mapsto_);
    }


}


#endif // FORM_HH__6D9552F4_CAB9_11E4_A9EC_001C42000009

// Form.hh ends here
