// -*- mode: C++ -*-
// Time-stamp: "2015-08-20 21:18:49 sb"

/*
  file       OutputManipulator.hh
  copyright  (c) Sebastian Blatt 2012, 2013, 2014, 2015

  std::ostream manipulators that restore the original stream state
  after emitting their argument in a particular way.

 */


#ifndef OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE
#define OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <stdint.h>
#include <sstream>
#include <type_traits>
#include <memory>
#include <limits>

// Base class for custom output manipulators, implements
// UNWIND-PROTECT for stream format state.
class OutputManipulator {
  public:
    virtual std::ostream& output(std::ostream& out) const = 0;
    std::ostream& output_wrapper(std::ostream& out) const {
      if(out.good()){
        std::ios_base::fmtflags flags = out.flags();
        char f = out.fill();
        std::streamsize sz = out.precision();
        output(out);
        out.precision(sz);
        out.fill(f);
        out.flags(flags);
      }
      return out;
    }
};
inline std::ostream& operator<<(std::ostream& out, const OutputManipulator& o) {
  return o.output_wrapper(out);
}

// Print a hexadecimal number in 0x0001ABCD format, padded with zeros
// according to byte width of type T.
template <typename T>
class hex_form : public OutputManipulator {
  private:
    const T t;
    const size_t n;
  public:
    hex_form(T t_, size_t n_ = sizeof(T)) : t(t_), n(n_) {}
    std::ostream& output(std::ostream& out) const {
      out << "0x" << std::setw(2 * n) << std::setfill('0')
          << std::hex
      // need to convert uint8_t, otherwise will print as a character
      // instead of a number.
          << ((n > sizeof(uint8_t)) ? t : static_cast<unsigned int>(t));
      return out;
    }
};

// Print an unsigned integer in spaced hexadecimal form (12 ab 32 0c),
// padded with zeros according to byte width of type T.
template <typename T>
class hex_split_form : public OutputManipulator {
  private:
    const T t;
    const size_t n;
  public:
    hex_split_form(T t_, size_t n_ = sizeof(T)) : t(t_), n(n_) {}
    std::ostream& output(std::ostream& out) const {
      out.fill('0');
      for(size_t j=0; j<n; ++j){
        uint16_t byte = (t >> 8*(n-1-j)) & 0x00ff;
        out << std::setw(2) << std::hex << byte;
        if(j < n-1){
          out << " ";
        }
      }
      return out;
    }
};

// Print boolean as "y" or "n"
class bool_yn_form : public OutputManipulator {
  private:
    const bool b;
    const bool capitalize;
  public:
    bool_yn_form(bool b_, bool capitalize_=false)
      : b(b_), capitalize(capitalize_) {}
    std::ostream& output(std::ostream& out) const {
      out << (b ? (capitalize ? "Y" : "y") : (capitalize ? "N" : "n"));
      return out;
    }
};

// Print boolean as 1 or 0
class bool_numeric_form : public OutputManipulator {
  private:
    const bool b;
  public:
    bool_numeric_form(bool b_) : b(b_) {}
    std::ostream& output(std::ostream& out) const {
      out << (b ? "1" : "0");
      return out;
    }
};

// Right-justify type T using field width N and fill character FILL.
template <typename T>
class right_justified : public OutputManipulator {
  private:
    const T t;
    const size_t n;
    const char fill;
  public:
    right_justified(T t_, size_t n_, char fill_ = ' ')
      : t(t_), n(n_), fill(fill_)
    {}
    std::ostream& output(std::ostream& out) const {
      out.fill(fill);
      out << std::setw(n) << t;
      return out;
    }
};

// Print any iterable container with size() member function as a
// comma-separated list.
template <typename IterableContainer>
class list_form : public OutputManipulator {
  protected:
    const IterableContainer& ic;
    const std::string& separator;
  public:
    list_form(const IterableContainer& ic_,
              const std::string& separator_ = ", ")
      : ic(ic_), separator(separator_)
    {}
    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const size_t n = ic.size();
      for(typename IterableContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        out << *i;
        if(j < n - 1){
          out << separator;
        }
      }
      return out;
    }
};

template <typename IterableContainer>
class ptr_list_form : public OutputManipulator {
  protected:
    const IterableContainer& ic;
    const std::string& separator;
  public:
    ptr_list_form(const IterableContainer& ic_,
                  const std::string& separator_ = ", ")
      : ic(ic_), separator(separator_)
    {}
    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const size_t n = ic.size();
      for(typename IterableContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        out << **i;
        if(j < n - 1){
          out << separator;
        }
      }
      return out;
    }
};

// Print any iterable container with size() member function as a
// comma-separated list, but only show the first and last elements
// for large lists.
template <typename IterableContainer>
class condensed_list_form : public list_form<IterableContainer> {
  protected:
    const size_t n_first;
    const size_t n_last;
    const std::string condensed_indicator;
  public:
    condensed_list_form(const IterableContainer& ic_,
                        const std::string& separator_ = ", ",
                        const size_t n_first_ = 3,
                        const size_t n_last_ = 2,
                        const std::string& condensed_indicator_ = "...")
      : list_form<IterableContainer>(ic_, separator_),
        n_first(n_first_),
        n_last(n_last_),
        condensed_indicator(condensed_indicator_)
    {}

    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const IterableContainer& ic = list_form<IterableContainer>::ic;
      const size_t n = ic.size();
      for(typename IterableContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        if(j < n_first || j >= ic.size() - n_last){
          out << *i;
          if(j < n - 1){
            out << list_form<IterableContainer>::separator;
          }
        }
        else{
          out << condensed_indicator;
        }
      }
      return out;
    }
};

template <typename T>
class identity_form : public OutputManipulator {
  private:
    const T& t;
  public:
    identity_form(const T& t_)
      : t(t_)
    {}
    std::ostream& output(std::ostream& out) const {
      out << t;
      return out;
    }
};

// FIXME: !!!
//
// This does not work yet. Problem is that we want to be able to say
// "Print a dictionary (or list), and use a particular form for the
// keys and another one for the values." This is fine as long as the
// form does not have any arguments besides the thing to spit out.
// (works with identity_form, for instance).
//
// Otherwise, would have to know all the arguments to the child forms
// at compile time and pass them through as template arguments. This
// means that the whole framework here has to change and we also do
// not always want that or know the arguments at compile time.
//
// Other options? Somehow would need to pass through an instantiated
// form object but without being linked to a thing to output already.
// This would also change the framework here, because the
// OutputManipulators have to become functors called with operator()
// of the type form(arguments)(const T& t) instead of taking T as an
// argument to the constructor.
//
// Only other way to output non-standard types then is to define
// operator<< for them which ties the program to a particular output
// form.

// Print any dictionary container with size() member function and
// iterator->first, iterator->second as a comma-separated list.
template <typename DictionaryContainer,
          typename KeyForm = identity_form<typename DictionaryContainer::key_type>,
          typename ValueForm = identity_form<typename DictionaryContainer::value_type>
          >
class dict_form : public OutputManipulator {
  protected:
    const DictionaryContainer& ic;
    const KeyForm& key_form;
    const ValueForm& value_form;
    const std::string& map_indicator;
    const std::string& separator;
  public:
    dict_form(const DictionaryContainer& ic_,
              const KeyForm& key_form_,
              const ValueForm& value_form_,
              const std::string& map_indicator_ = " -> ",
              const std::string& separator_ = ", ")
      : ic(ic_),
        key_form(key_form_),
        value_form(value_form_),
        map_indicator(map_indicator_),
        separator(separator_)
    {}
    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const size_t n = ic.size();
      for(typename DictionaryContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        out << KeyForm(i->first) << map_indicator << ValueForm(i->second);
        if(j < n - 1){
          out << separator;
        }
      }
      return out;
    }
};

// Print any dictionary container with size() member function and
// iterator->first, iterator->second as a comma-separated list, but
// only show the first and last elements for large dictionaries.
template <typename DictionaryContainer,
          typename KeyForm = identity_form<typename DictionaryContainer::key_type>,
          typename ValueForm = identity_form<typename DictionaryContainer::value_type>
          >
class condensed_dict_form : public dict_form<DictionaryContainer,
                                             KeyForm,
                                             ValueForm>
{
  public:
    typedef dict_form<DictionaryContainer, KeyForm, ValueForm> base_type_t;
  protected:
    const size_t n_first;
    const size_t n_last;
    const std::string condensed_indicator;
  public:
    condensed_dict_form(const DictionaryContainer& ic_,
                        const std::string& map_indicator_ = " -> ",
                        const std::string& separator_ = ", ",
                        const size_t n_first_ = 3,
                        const size_t n_last_ = 2,
                        const std::string& condensed_indicator_ = "...")
      : base_type_t(ic_, map_indicator_, separator_),
        n_first(n_first_),
        n_last(n_last_),
        condensed_indicator(condensed_indicator_)
    {}

    std::ostream& output(std::ostream& out) const {
      size_t j = 0;
      const DictionaryContainer& ic = base_type_t::ic;
      const size_t n = ic.size();
      for(typename DictionaryContainer::const_iterator i = ic.begin();
          i != ic.end(); ++i, ++j)
      {
        if(j < n_first || j >= ic.size() - n_last){
          out << i->first
              << base_type_t::map_indicator
              << i->second;
          if(j < n - 1){
            out << base_type_t::separator;
          }
        }
        else{
          out << condensed_indicator;
        }
      }
      return out;
    }
};


// Print std::vector as list
template <typename T>
class vector_form : public OutputManipulator {
  private:
    const std::vector<T>& v;
  public:
    vector_form(const std::vector<T>& v_)
      : v(v_)
    {}
    std::ostream& output(std::ostream& out) const {
      out << "(" << list_form< std::vector<T> >(v, ", ") << ")";
      return out;
    }
};

template <typename T, typename Pointer = std::unique_ptr<T> >
class ptr_vector_form : public OutputManipulator {
  private:
    const std::vector< Pointer >& v;
  public:
    ptr_vector_form(const std::vector< Pointer >& v_)
      : v(v_)
    {}
    std::ostream& output(std::ostream& out) const {
      out << "(" << ptr_list_form< std::vector< Pointer > >(v, ", ") << ")";
      return out;
    }
};


// Print std::array as list
template <typename T, size_t N>
class array_form : public OutputManipulator {
  private:
    const std::array<T, N>& v;
  public:
    array_form(const std::array<T, N>& v_)
      : v(v_)
    {}
    std::ostream& output(std::ostream& out) const {
      out << "{" << list_form< std::array<T, N> >(v, ", ") << "}";
      return out;
    }
};



// Print std::vector of unsigned integers as list of hex numbers with addresses
template <typename T>
class hex_vector_form : public OutputManipulator {
  private:
    const std::vector<T>& v;
    const size_t w;
  public:
    hex_vector_form(const std::vector<T>& v_, size_t w_=20/sizeof(T))
      : v(v_), w(w_) {}
    std::ostream& output(std::ostream& out) const {
      for(size_t i=0; i<v.size(); ++i){
        if(i % w == 0){
          out << "  " << right_justified<size_t>(i, 6) << "  ";
        }
        out << hex_split_form<T>(v[i]);
        if(i % w == w-1){
          out << "\n";
        }
        else{
          out << ((sizeof(T)>1) ? "  " : " ");
        }
      }
      return out;
    }
};

// If numerator % denominator == 0, print (numerator/denominator),
// else print numerator << "/" << denominator.
template <typename T>
class fractional_form : public OutputManipulator {
  private:
    const T numerator;
    const T denominator;
  public:
    fractional_form(const T& numerator_,
                    const T& denominator_)
      : numerator(numerator_),
        denominator(denominator_)
    {}
    std::ostream& output(std::ostream& out) const {
      if(numerator % denominator == 0){
        out << numerator / denominator;
      }
      else{
        out << numerator << "/" << denominator;
      }
      return out;
    }
};

// Print long integer with three digit separator, e.g. 1,000,000
// instead of 1000000.
template <typename T>
class integer_separated_form : public OutputManipulator {
  private:
    const T number;
    const std::string separator;
  public:
    integer_separated_form(const T& number_,
                           const std::string& separator_ = " ")
      : number(number_),
        separator(separator_)
    {}
    std::ostream& output(std::ostream& out) const {
      std::ostringstream os;
      os << number;
      std::string s = os.str();
      size_t n = s.size();

      size_t pos = 0;
      if(n % 3 != 0){
        out << s.substr(0, n%3) << separator;
        pos = n%3;
      }
      for(size_t j = 0; j < n/3 - 1; ++j){
        out << s.substr(pos, 3) << separator;
        pos += 3;
      }
      out << s.substr(pos);
      return out;
    }
};

// Print double in decimal with highest precision allowed by platform.
// We would want to do this to ensure that reading back a text file
// results in the same double precision number. See
//
//   https://stackoverflow.com/questions/4643641/best-way-to-output-a-full-precision-double-into-a-text-file
//
// Ideally, would use HDF5 to store floating point data in defined
// binary format.
//
class double_full_precision_form : public OutputManipulator {
  private:
    const double number;
  public:
    double_full_precision_form(const double number_)
      : number(number_)
    {}

    std::ostream& output(std::ostream& out) const {
      out.precision(std::numeric_limits<int>::max());
      out << number;
      return out;
    }
};


#endif // OUTPUTMANIPULATOR_HH__F059345E_2A18_4FD8_85C8_5D78AA02D4FE

// OutputManipulator.hh ends here
