///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    Point2D.cpp                                                      //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "Point2D.hpp"

namespace geometry {
  /////////////////////////////////////////////////////////////////////////////
  // Point2D implementation                                                  //
  /////////////////////////////////////////////////////////////////////////////
  Point2D Point2D::operator-(const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
  }

  coord_t Point2D::crossProduct(const Point2D& a, const Point2D& b) {
    return (a.x*b.y)-(b.x*a.y);
  }

  bool Point2D::leftTurn(const Point2D& a, const Point2D& b, const Point2D& c) {
    return crossProduct(c-a,b-a) < 0;
  }

  bool Point2D::rightTurn(const Point2D& a, const Point2D& b, const Point2D& c) {
    return crossProduct(c-a,b-a) > 0;
  }

  bool Point2D::colinear(const Point2D& a, const Point2D& b, const Point2D& c) {
    return crossProduct(c-a,b-a) == 0;
  }
    
  
  bool operator>(const Point2D& a, const Point2D& b) {
    return a.x > b.x;
  }

  bool operator>=(const Point2D& a, const Point2D& b) {
    return !(operator<(a,b));
  }

  bool operator<(const Point2D& a, const Point2D& b) {
    return a.x < b.x;
  }

  bool operator<=(const Point2D& a, const Point2D& b) {
    return !(operator>(a,b));
  }

  bool operator==(const Point2D& a, const Point2D& b) {
    return (a.x == b.x) && (a.y == b.y);
  }
  
  ostream& operator<<(ostream& os, const Point2D& p) {
    os << p.x << " " << p.y;
    return os;
  }

  istream& operator>>(istream& is, Point2D& p) {
    is >> p.x >> p.y;
    return is;
  }
}
namespace std {
  /////////////////////////////////////////////////////////////////////////////
  // Define limits for points                                                //
  /////////////////////////////////////////////////////////////////////////////
  geometry::Point2D numeric_limits<geometry::Point2D>::infinity() {
    assert(numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::infinity(),
			     numeric_limits<geometry::coord_t>::infinity());
  }
  geometry::Point2D numeric_limits<geometry::Point2D>::max() {
    assert(! numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::max(),
			     numeric_limits<geometry::coord_t>::max());
  }
  geometry::Point2D numeric_limits<geometry::Point2D>::min() {
    assert(! numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::min(),
			     numeric_limits<geometry::coord_t>::min());
  }
}

