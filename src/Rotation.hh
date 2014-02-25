// -*- mode: C++ -*-
// Time-stamp: "2012-01-18 12:52:19 sb"

/*
  file       Rotation.hh
  copyright  (c) Sebastian Blatt 2012

 */


#ifndef ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8
#define ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8

#include <vector>
#include "Representable.hh"
#include "Point.hh"

class Rotation : public Representable {
  private:
    std::vector<double> data;
    size_t DataIndex(size_t row, size_t column) const;
    void SetElement(size_t row, size_t column, double x);

  public:
    Rotation();

    void Clear();
    void Identity() {Clear();}
    double GetElement(size_t row, size_t column) const;
    Point ApplyTo(const Point& p) const;

    void Around(const Point& p, double angle);
    void FromTo(const Point& u, const Point& v);
    double Trace() const;
    double GetAngle() const;

    std::ostream& Represent(std::ostream& out) const;
};

inline Point operator*(const Rotation& r, const Point& p) {return r.ApplyTo(p);}

#endif // ROTATION_HH__EF68D992_36D6_49FE_A0E9_5BCDC50F2AC8

// Rotation.hh ends here
