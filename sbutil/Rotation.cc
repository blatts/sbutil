// -*- mode: C++ -*-
// Time-stamp: "2016-06-07 13:16:57 sb"

/*
  file       Rotation.cc
  copyright  (c) Sebastian Blatt 2012 -- 2016

 */

#include <cmath>
#include <sstream>

#include <sbutil/Exception.hh>
#include <sbutil/Rotation.hh>
#include <sbutil/Const.hh>

size_t Rotation::DataIndex(size_t row, size_t column) const {
  if(row > 3 || column > 3){
    std::ostringstream os;
    os << "Element indices (" << row << ", " << column
       << ") not in valid index range ([0,3], [0,3])";
    throw EXCEPTION(os.str());
  }
  return 3*row + column;
}

void Rotation::SetElement(size_t row, size_t column, double x){
  size_t idx = DataIndex(row,column);
  data[idx] = x;
}


Rotation::Rotation()
  : data(9, 0.0)
{
  Clear();
}

void Rotation::Clear() {
  std::fill(data.begin(), data.end(), 0.0);
  for(size_t i=0; i<3; ++i){
    SetElement(i,i,1.0);
  }
}

double Rotation::GetElement(size_t row, size_t column) const {
  size_t idx = DataIndex(row, column);
  return data[idx];
}

Point Rotation::ApplyTo(const Point& p) const {
  Point rc;
  rc.x = GetElement(0,0)*p.x + GetElement(0,1)*p.y + GetElement(0,2)*p.z;
  rc.y = GetElement(1,0)*p.x + GetElement(1,1)*p.y + GetElement(1,2)*p.z;
  rc.z = GetElement(2,0)*p.x + GetElement(2,1)*p.y + GetElement(2,2)*p.z;
  return rc;
}

void Rotation::Around(const Point& p, double angle){
  double c = cos(angle);
  double cp = 1.0 - c;
  double s = sin(angle);
  Point n(p);
  n.Normalize();
  SetElement(0, 0, c + n.x*n.x*cp);
  SetElement(0, 1, n.x*n.y*cp - n.z*s);
  SetElement(0, 2, n.x*n.z*cp + n.y*s);
  SetElement(1, 0, n.y*n.x*cp + n.z*s);
  SetElement(1, 1, c + n.y*n.y*cp);
  SetElement(1, 2, n.y*n.z*cp - n.x*s);
  SetElement(2, 0, n.z*n.x*cp - n.y*s);
  SetElement(2, 1, n.z*n.y*cp + n.x*s);
  SetElement(2, 2, c + n.z*n.z*cp);
}

void Rotation::FromTo(const Point& u, const Point& v){
  Point axis = Cross(u,v);
  double angle = acos(u * v);
  Around(axis, angle);
}

void Rotation::FromZTo(const Point& u){
  static const Point ez(0, 0, 1);
  FromTo(ez, u);
}


double Rotation::Trace() const {
  return GetElement(0,0) + GetElement(1,1) + GetElement(2,2);
}

double Rotation::GetAngle() const {
  return acos(0.5 * (Trace() - 1.0));
}

std::ostream& Rotation::Represent(std::ostream& out) const {
  out << "Angle : " << GetAngle() << " rad = "
      << 180. / Const::pi * GetAngle() << " deg\n";
  for(size_t i=0; i<3; ++i){
    out << ((i==0) ? "/" : ((i==2) ? "\\" : "|")) << "\t";
    for(size_t j=0; j<3; ++j){
      out << GetElement(i,j);
      if(j<2){
        out << "\t";
      }
    }
    out << "\t" << ((i==0) ? "\\" : ((i==2) ? "/" : "|"));
    if(i<2){
      out << "\n";
    }
  }
  return out;
}




// Rotation.cc ends here
