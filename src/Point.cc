// -*- mode: C++ -*-
// Time-stamp: "2012-01-14 09:57:37 sb"

/*
  file       Point.cc
  copyright  (c) Sebastian Blatt 2012

 */

#include <cmath>

#include "Point.hh"

Point::Point(double x_, double y_, double z_)
  : x(x_), y(y_), z(z_)
{
}

Point::Point(const Point& b)
  : Representable(),
    x(b.x), y(b.y), z(b.z)
{
}

Point& Point::operator=(const Point& b){
  x = b.x;
  y = b.y;
  z = b.z;
  return *this;
}

bool Point::operator==(const Point& b) const{
  return (x == b.x) && (y == b.y) && (z == b.z);
}

double Point::LengthSquared() const {
  return x*x + y*y + z*z;
}

double Point::Length() const{
  return sqrt(LengthSquared());
}

void Point::Normalize(){
  double r = Length();
  if(r > 0.0){
    x /= r;
    y /= r;
    z /= r;
  }
}

double Point::Dot(const Point& b) const{
  return x*b.x + y*b.y + z*b.z;
}

Point Point::Cross(const Point& b) const{
  return Point(y*b.z - z*b.y,
               z*b.x - x*b.z,
               x*b.y - y*b.x);
}

Point& Point::operator*=(double scale){
  x *= scale;
  y *= scale;
  z *= scale;
  return *this;
}

Point& Point::operator/=(double scale){
  x /= scale;
  y /= scale;
  z /= scale;
  return *this;
}

Point& Point::operator+=(const Point& b){
  x += b.x;
  y += b.y;
  z += b.z;
  return *this;
}

Point& Point::operator-=(const Point& b){
  x -= b.x;
  y -= b.y;
  z -= b.z;
  return *this;
}

Point operator*(const Point& a, double scale){
  Point p = a;
  p *= scale;
  return p;
}

Point operator*(double scale, const Point& b){
  return b * scale;
}

double operator*(const Point& a, const Point& b){
  return a.Dot(b);
}

Point operator/(const Point& a, double scale){
  Point p = a;
  p /= scale;
  return p;
}

Point operator+(const Point& a, const Point& b){
  Point p = a;
  p += b;
  return p;
}

Point operator-(const Point& a, const Point& b){
  Point p = a;
  p -= b;
  return p;
}

Point operator-(const Point& a){
  return Point(-a.x, -a.y, -a.z);
}

std::ostream& Point::Represent(std::ostream& out) const{
  out << "(" << x << ", " << y << ", " << z << ")";
  return out;
}



// Point.cc ends here
