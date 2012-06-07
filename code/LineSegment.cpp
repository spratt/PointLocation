///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    LineSegment.cpp                                                  //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "LineSegment.hpp"

namespace geometry {

  LineSegment::LineSegment(int ax, int ay, int bx, int by)
    : first(ax,ay), second(bx,by)
  {
  }
  
  LineSegment::LineSegment(Point2D& a, Point2D& b)
    : first(a), second(b)
  {
  }

  LineSegment::~LineSegment() {}

  const Point2D& LineSegment::getFirstEndPoint() const { return first; }
  const Point2D& LineSegment::getSecondEndPoint() const { return second; }

  bool LineSegment::operator<(const LineSegment& other) const {
    return ydesc(*this,other);
  }

  bool LineSegment::operator>(const LineSegment& other) const {
    return yasc(*this,other);
  }

  bool LineSegment::ydesc(const LineSegment& first, const LineSegment& other) {
    // sort by y coordinate descending
    const int max_y       = (first.first.y > first.second.y) ?
      first.first.y : first.second.y;
    const int other_max_y = (other.first.y > other.second.y) ?
      other.first.y : other.second.y;
    // first precedes other iff first segment's y is higher
    return max_y > other_max_y;
  }

  bool LineSegment::yasc(const LineSegment& first, const LineSegment& other) {
    return !ydesc(first,other);
  }
  
  bool LineSegment::xdesc(const LineSegment& first, const LineSegment& other) {
    // sort by x coordinate descending
    const int max_x       = (first.first.x > first.second.x) ?
      first.first.x : first.second.x;
    const int other_max_x = (other.first.x > other.second.x) ?
      other.first.x : other.second.x;
    // first precedes other iff first segment's x is higher
    return max_x > other_max_x;
  }

  bool LineSegment::xasc(const LineSegment& first, const LineSegment& other) {
    return !xdesc(first,other);
  }

  ostream& operator<<(ostream& os, const LineSegment& ls) {
    return os << ls.getFirstEndPoint() << " " << ls.getSecondEndPoint();
  }
  
  istream& operator>>(istream& is, LineSegment& ls) {
    return is >> ls.first >> ls.second;
  }
}
