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

// DEBUG
#include <iostream>
using namespace std;

namespace geometry {

  LineSegment::LineSegment(int ax, int ay, int bx, int by)
    : first(ax,ay), second(bx,by)
  {
  }
  
  LineSegment::LineSegment(Point2D& a, Point2D& b)
    : first(a), second(b)
  {
  }
  
  LineSegment::LineSegment(const Point2D& a, const Point2D& b)
    : first(a), second(b)
  {
  }

  LineSegment::~LineSegment() {}

  const Point2D& LineSegment::getFirstEndPoint() const { return first; }
  const Point2D& LineSegment::getSecondEndPoint() const { return second; }

  const Point2D& LineSegment::getLeftEndPoint() const {
    return (first.x < second.x)?first:second;
  }
  const Point2D& LineSegment::getRightEndPoint() const {
    return (first.x > second.x)?first:second;
  }
  const Point2D& LineSegment::getTopEndPoint() const {
    return (first.y > second.y)?first:second;
  }
  const Point2D& LineSegment::getBottomEndPoint() const {
    return (first.y < second.y)?first:second;
  }

  bool LineSegment::operator<(const LineSegment& other) const {
    return ydesc(*this,other);
  }

  bool LineSegment::operator>(const LineSegment& other) const {
    return yasc(*this,other);
  }

  // returns true if first belongs before other in ascending order of y
  bool LineSegment::ydesc(const LineSegment& first, const LineSegment& other) {
    bool firstIsPoint = first.getFirstEndPoint() == first.getSecondEndPoint();
    bool otherIsPoint = other.getFirstEndPoint() == other.getSecondEndPoint();
    if((firstIsPoint && otherIsPoint) || ((!firstIsPoint) && (!otherIsPoint)))
      // simply compare y coordinates
      return (first.getFirstEndPoint().y > other.getFirstEndPoint().y)
	|| (first.getSecondEndPoint().y > other.getSecondEndPoint().y);
    else if(firstIsPoint)
      return Point2D::leftTurn(other.getLeftEndPoint(),
			       other.getRightEndPoint(),
			       first.getFirstEndPoint());
    else if(otherIsPoint)
      return Point2D::rightTurn(first.getLeftEndPoint(),
			       first.getRightEndPoint(),
			       other.getFirstEndPoint());
    throw "Boolean logic has failed!  ABORT!";
  }

  bool LineSegment::yasc(const LineSegment& first, const LineSegment& other) {
    bool firstIsPoint = first.getFirstEndPoint() == first.getSecondEndPoint();
    bool otherIsPoint = other.getFirstEndPoint() == other.getSecondEndPoint();
    if((firstIsPoint && otherIsPoint) || ((!firstIsPoint) && (!otherIsPoint))) {
      // simply compare y coordinates
      return (first.getFirstEndPoint().y < other.getFirstEndPoint().y)
	|| (first.getSecondEndPoint().y < other.getSecondEndPoint().y);
    } else if(firstIsPoint) {
      return Point2D::rightTurn(other.getLeftEndPoint(),
			       other.getRightEndPoint(),
			       first.getFirstEndPoint());
    } else if(otherIsPoint) {
      return Point2D::leftTurn(first.getLeftEndPoint(),
				first.getRightEndPoint(),
				other.getFirstEndPoint());
    }
    throw "Boolean logic has failed!  ABORT!";
  }
  
  bool LineSegment::xdesc(const LineSegment& first, const LineSegment& other) {
    return (first.getTopEndPoint() > other.getTopEndPoint());
  }

  bool LineSegment::xasc(const LineSegment& first, const LineSegment& other) {
    return (first.getTopEndPoint() < other.getTopEndPoint());
  }

  ostream& operator<<(ostream& os, const LineSegment& ls) {
    return os << ls.getFirstEndPoint() << " " << ls.getSecondEndPoint();
  }
  
  istream& operator>>(istream& is, LineSegment& ls) {
    return is >> ls.first >> ls.second;
  }

  bool LineSegment::operator==(const LineSegment& other) const {
    return ((first == other.first) && (second == other.second))
      || ((first == other.second) && (second == other.first));
  }
}
