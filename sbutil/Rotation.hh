// -*- mode: C++ -*-
// Time-stamp: "2016-06-07 13:16:45 sb"

/*
  file       Rotation.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */


#ifndef ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8
#define ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8

#include <vector>
#include <sbutil/Representable.hh>
#include <sbutil/Point.hh>

class Rotation : public Representable {
  private:
    std::vector<double> data;
    size_t DataIndex(size_t row, size_t column) const;
    void SetElement(size_t row, size_t column, double x);

  public:
    Rotation();
    Rotation(const Point& p, double angle) : data(9, 0.0) {Around(p, angle);}
    Rotation(const Point& u, const Point& v) : data(9, 0.0) {FromTo(u, v);}
    Rotation(const Point& u) : data(9, 0.0){FromZTo(u);}

    void Clear();
    void Identity() {Clear();}
    double GetElement(size_t row, size_t column) const;
    Point ApplyTo(const Point& p) const;

    void Around(const Point& p, double angle);
    void FromTo(const Point& u, const Point& v);
    void FromZTo(const Point& u);
    double Trace() const;
    double GetAngle() const;

    std::ostream& Represent(std::ostream& out) const;
};

inline Point operator*(const Rotation& r, const Point& p) {return r.ApplyTo(p);}

#endif // ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8

// Rotation.hh ends here
