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

#include <cassert>
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
    return (first.x < second.x)?second:first;
  }
  const Point2D& LineSegment::getTopEndPoint() const {
    return (first.y > second.y)?first:second;
  }
  const Point2D& LineSegment::getBottomEndPoint() const {
    return (first.y > second.y)?second:first;
  }

  const bool LineSegment::isVertical() const {
    return first.x == second.x;
  }

  IntersectionResult LineSegment::intersection(const LineSegment& other) const {
    ///////////////////////////////////////////////////////////////////////////
    // From the theory on:                                                   //
    // http://paulbourke.net/geometry/lineline2d/                            //
    ///////////////////////////////////////////////////////////////////////////
    IntersectionResult result;
    coord_t denom = Point2D::crossProduct(second-first,other.second-other.first);
    coord_t num_a = Point2D::crossProduct(other.second-other.first,first-other.first);
    coord_t num_b = Point2D::crossProduct(second-first,first-other.first);

    if(denom == 0) {
      if(num_a == 0 && num_b == 0)
	result.isCoincident = true;
      else
	result.isParallel = true;
      return result;
    }

    coord_t ua = num_a / denom;
    coord_t ub = num_b / denom;

    if(ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {
      result.point = Point2D(first.x + ua * (second.x - first.x),
			     first.y + ua * (second.y - first.y));
      result.isIntersecting = true;
    }
    return result;
  }

  bool LineSegment::operator<(const LineSegment& other) const {
    // first sort by y
    if(ydesc(*this,other)) return true;
    if(yasc(*this,other)) return false;
    // otherwise, sort by x
    if(xdesc(*this,other)) return true;
    if(xasc(*this,other)) return false;
    // otherwise must be equal?
    return false;
  }

  bool LineSegment::operator>(const LineSegment& other) const {
    return !((*this) == other) && !((*this) < other);
  }

  // returns true if first belongs before other in ascending order of y
  bool LineSegment::ydesc(const LineSegment& a, const LineSegment& b) {
    if(a == b) return false;

    ///////////////////////////////////////////////////////////////////////////
    // Point Cases                                                           //
    ///////////////////////////////////////////////////////////////////////////
    bool a_is_point = a.getFirstEndPoint() == a.getSecondEndPoint();
    bool b_is_point = b.getFirstEndPoint() == b.getSecondEndPoint();
    if(a_is_point && b_is_point)
      return
	(a.getFirstEndPoint().y > b.getFirstEndPoint().y) ||
	(a.getFirstEndPoint().y == b.getFirstEndPoint().y &&
	 a.getFirstEndPoint().x > b.getFirstEndPoint().x);
    if(a_is_point)
      return Point2D::leftTurn(b.getLeftEndPoint(),
			       b.getRightEndPoint(),
			       a.getFirstEndPoint());
    if(b_is_point)
      return Point2D::rightTurn(a.getLeftEndPoint(),
				a.getRightEndPoint(),
				b.getFirstEndPoint());
    
    bool shared_left_end_point =
      a.getLeftEndPoint() == b.getLeftEndPoint();
    bool shared_right_end_point =
      a.getRightEndPoint() == b.getRightEndPoint();
    bool a_right_b_left =
      a.getRightEndPoint() == b.getLeftEndPoint();
    bool a_left_b_right =
      a.getLeftEndPoint() == b.getRightEndPoint();

    ///////////////////////////////////////////////////////////////////////////
    // Vertical Cases                                                        //
    ///////////////////////////////////////////////////////////////////////////
    if(a.isVertical() && b.isVertical())
      return a.getTopEndPoint().y > b.getTopEndPoint().y;

    if(a.isVertical()) {
      if(shared_left_end_point ||
	 shared_right_end_point ||
	 a_right_b_left ||
	 a_left_b_right)
	return
	  (a.getBottomEndPoint() == b.getLeftEndPoint()) ||
	  (a.getBottomEndPoint() == b.getRightEndPoint());
    }
    if(b.isVertical()) {
      if(shared_left_end_point ||
	 shared_right_end_point ||
	 a_right_b_left ||
	 a_left_b_right)
	return
	  (b.getTopEndPoint() == a.getLeftEndPoint()) ||
	  (b.getTopEndPoint() == a.getRightEndPoint());
    }

    ///////////////////////////////////////////////////////////////////////////
    // Shared End Point Cases                                                //
    ///////////////////////////////////////////////////////////////////////////
    if(shared_left_end_point)
      return Point2D::leftTurn(a.getRightEndPoint(),
			       a.getLeftEndPoint(),
			       b.getRightEndPoint());
    if(shared_right_end_point)
      return Point2D::rightTurn(a.getLeftEndPoint(),
				a.getRightEndPoint(),
				b.getLeftEndPoint());
    if(a_right_b_left)
      return
	(a.getLeftEndPoint().y > b.getRightEndPoint().y) ||
	(a.getLeftEndPoint().y == b.getRightEndPoint().y &&
	 a.getLeftEndPoint().x > b.getRightEndPoint().x);
    if(a_left_b_right)
      return
	(a.getRightEndPoint().y > b.getLeftEndPoint().y) ||
	(a.getRightEndPoint().y == b.getLeftEndPoint().y &&
	 a.getRightEndPoint().x > b.getLeftEndPoint().x);
    
    ///////////////////////////////////////////////////////////////////////////
    // Basic Cases                                                           //
    ///////////////////////////////////////////////////////////////////////////
    return
      (Point2D::rightTurn(a.getLeftEndPoint(),
			  a.getRightEndPoint(),
			  b.getFirstEndPoint())
       &&
       Point2D::rightTurn(a.getLeftEndPoint(),
			  a.getRightEndPoint(),
			  b.getSecondEndPoint()))
      ||
      (Point2D::leftTurn(b.getLeftEndPoint(),
			 b.getRightEndPoint(),
			 a.getFirstEndPoint())
       &&
       Point2D::leftTurn(b.getLeftEndPoint(),
			 b.getRightEndPoint(),
			 a.getSecondEndPoint()));
  }

  bool LineSegment::yasc(const LineSegment& a, const LineSegment& b) {
    return a != b && !ydesc(a,b);
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

  bool LineSegment::operator!=(const LineSegment& other) const {
    return !((*this) == other);
  }
}
