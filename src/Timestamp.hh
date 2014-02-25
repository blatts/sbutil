// -*- mode: C++ -*-
// Time-stamp: "2012-08-07 14:08:35 sb"

/*
  file       Timestamp.hh
  copyright  (c) Sebastian Blatt 2012

 */


#ifndef TIMESTAMP_HH__54390E81_90BF_4071_BA85_38D20483C9A5
#define TIMESTAMP_HH__54390E81_90BF_4071_BA85_38D20483C9A5

#include "Representable.hh"
#include <stdint.h>

// Data type representing unsigned integer absolute time since UNIX
// Epoch in units of seconds plus unsigned integer microsecond offset.
class Timestamp : public Representable {
  private:
    uint32_t stamp;
    uint32_t microseconds;
  public:
    Timestamp();
    Timestamp(uint32_t stamp_, uint32_t microseconds_) { Set(stamp_, microseconds_);}
    Timestamp(const Timestamp& x) : Representable() {Set(x);}
    const Timestamp& operator=(const Timestamp& x){
      Set(x);
      return *this;
    }

    std::ostream& Represent(std::ostream& out) const;
    void Now();
    void At(int year, int month, int day,
            int hour, int minute, int second,
            int msecond, int usecond);
    uint32_t GetSeconds() const {return stamp;}
    uint32_t GetMicroSeconds() const {return microseconds;}
    void Set(uint32_t stamp_, uint32_t microseconds_){
      stamp = stamp_;
      microseconds = microseconds_;
    }
    void Set(const Timestamp& x){
      stamp = x.GetSeconds();
      microseconds = x.GetMicroSeconds();
    }
};

// Data type representing double accuracy relative time in units of seconds
class RelativeTime : public Representable {
  private:
    double offset;
  public:
    RelativeTime(double offset_ = 0.0);
    RelativeTime(const RelativeTime& x) : Representable() {Set(x);}
    const RelativeTime& operator=(const RelativeTime& x){
      Set(x);
      return *this;
    }

    std::ostream& Represent(std::ostream& out) const;
    double Get() const {return offset;}
    void Set(double offset_) {offset = offset_;}
    void Set(const RelativeTime& x) {offset = x.Get();}

    bool operator<(const RelativeTime& x) const {return offset < x.Get();}
    bool operator>=(const RelativeTime& x) const {return !operator<(x);}
    bool operator<(double x) const {return offset < x;}
    bool operator>=(double x) const {return !operator<(x);}
    bool operator==(const RelativeTime& x) const {return offset == x.Get();}
};

inline bool operator<(double a, const RelativeTime& b) {return a < b.Get();}
inline bool operator>(double a, const RelativeTime& b) {return a > b.Get();}

RelativeTime operator-(const Timestamp& a, const Timestamp& b);
Timestamp operator+(const Timestamp& a, const RelativeTime& b);
inline Timestamp operator+(const RelativeTime& a, const Timestamp& b) {return b + a;}



#endif // TIMESTAMP_HH__54390E81_90BF_4071_BA85_38D20483C9A5

// Timestamp.hh ends here
