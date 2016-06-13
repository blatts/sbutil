// -*- mode: C++ -*-
// Time-stamp: "2016-06-12 13:46:53 sb"

/*
  file       Point.hh
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */


#ifndef POINT_HH__48B46E33_CEA4_4B97_A306_9CAA24E9961B
#define POINT_HH__48B46E33_CEA4_4B97_A306_9CAA24E9961B

#include <sbutil/Representable.hh>

class Point : public Representable {
  public:
    double x;
    double y;
    double z;

    Point(double x_=0.0, double y_=0.0, double z_=0.0);
    Point(const double theta, const double phi);
    Point(const Point& b);
    Point& operator=(const Point& b);

    bool operator==(const Point& b) const;

    void FromSpherical(const double theta, const double phi);

    double Length() const;
    double LengthSquared() const;
    void Normalize();
    double Dot(const Point& b) const;
    Point Cross(const Point& b) const;

    Point& operator*=(double scale);
    Point& operator/=(double scale);
    Point& operator+=(const Point& b);
    Point& operator-=(const Point& b);

    std::ostream& Represent(std::ostream& out) const;
    std::ostream& RepresentSeparated(std::ostream& out,
                                     const std::string& separator = "\t") const;

};

Point operator*(const Point& a, double scale);
Point operator*(double scale, const Point& b);
double operator*(const Point& a, const Point& b);
Point operator/(const Point& a, double scale);
Point operator+(const Point& a, const Point& b);
Point operator-(const Point& a, const Point& b);
Point operator-(const Point& a);

inline double Dot(const Point& a, const Point& b){
  return a * b;
}
inline Point Cross(const Point& a, const Point& b){
  Point c = a;
  return c.Cross(b);
}


#endif // POINT_HH__48B46E33_CEA4_4B97_A306_9CAA24E9961B

// Point.hh ends here
