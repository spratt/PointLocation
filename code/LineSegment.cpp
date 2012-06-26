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
    build();
  }
  
  LineSegment::LineSegment(Point2D& a, Point2D& b)
    : first(a), second(b)
  {
    if(a.x < b.x) {
      left = a;
      right = b;
    } else {
      left = b;
      right = a;
    }
    if(a.y < b.y) {
      bottom = a;
      top = b;
    } else {
      bottom = b;
      top = a;
    }
  }
  
  LineSegment::LineSegment(const Point2D& a, const Point2D& b)
    : first(a), second(b)
  {
    if(a.x < b.x) {
      left = a;
      right = b;
    } else {
      left = b;
      right = a;
    }
    if(a.y < b.y) {
      bottom = a;
      top = b;
    } else {
      bottom = b;
      top = a;
    }
  }

  LineSegment::~LineSegment() {}

  const Point2D& LineSegment::getFirstEndPoint()  const { return first;  }
  const Point2D& LineSegment::getSecondEndPoint() const { return second; }

  const Point2D& LineSegment::getLeftEndPoint()   const { return left;   }
  const Point2D& LineSegment::getRightEndPoint()  const { return right;  }
  const Point2D& LineSegment::getTopEndPoint()    const { return top;    }
  const Point2D& LineSegment::getBottomEndPoint() const { return bottom; }

  const bool LineSegment::isVertical() const {
    return first.x == second.x;
  }

  const bool LineSegment::isHorizontal() const {
    return first.y == second.y;
  }

  IntersectionResult LineSegment::intersection(const LineSegment& other) const {
    ///////////////////////////////////////////////////////////////////////////
    // From the theory on:                                                   //
    // http://paulbourke.net/geometry/lineline2d/                            //
    ///////////////////////////////////////////////////////////////////////////
    IntersectionResult result;
    coord_t denom = Point2D::crossProduct(second-first,other.second-other.first);
    coord_t num_a = Point2D::crossProduct(other.second-other.first,
					  first-other.first);
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
    bool yasc_flag, ydesc_flag, xasc_flag, xdesc_flag;
    if(this->getLeftEndPoint().x < other.getLeftEndPoint().x) {
      ydesc_flag = ydesc(*this,other);
      yasc_flag = yasc(*this,other);
    } else {
      yasc_flag = ydesc(other,*this);
      ydesc_flag = yasc(other,*this);
    }
    if(this->getBottomEndPoint().y < other.getBottomEndPoint().y) {
      xdesc_flag = xdesc(*this,other);
      xasc_flag = xasc(*this,other);
    } else {
      xasc_flag = xdesc(other,*this);
      xdesc_flag = xasc(other,*this);
    }
    if(ydesc_flag)
      return true;
    if(yasc_flag)
      return false;
    if(xdesc_flag)
      return true;
    // xasc or equal
    return false;
  }

  bool LineSegment::operator>(const LineSegment& other) const {
    bool yasc_flag, ydesc_flag, xasc_flag, xdesc_flag;
    if(this->getLeftEndPoint().x < other.getLeftEndPoint().x) {
      ydesc_flag = ydesc(*this,other);
      yasc_flag = yasc(*this,other);
    } else {
      yasc_flag = ydesc(other,*this);
      ydesc_flag = yasc(other,*this);
    }
    if(this->getBottomEndPoint().y < other.getBottomEndPoint().y) {
      xdesc_flag = xdesc(*this,other);
      xasc_flag = xasc(*this,other);
    } else {
      xasc_flag = xdesc(other,*this);
      xdesc_flag = xasc(other,*this);
    }
    if(yasc_flag)
      return true;
    if(ydesc_flag)
      return false;
    if(xasc_flag)
      return true;
    // xdesc or equal
    return false;
  }

  bool LineSegment::operator<=(const LineSegment& other) const {
    return !operator>(other);
  }

  bool LineSegment::operator>=(const LineSegment& other) const {
    return !operator<(other);
  }

  // returns true if left belongs above right in descending order
  bool LineSegment::ydesc(const LineSegment& left, const LineSegment& right) {
    // both vertical
    if(left.isVertical() && right.isVertical())
      return left.getBottomEndPoint().y > right.getBottomEndPoint().y;
    // left is vertical
    if(left.isVertical())
      return left.getBottomEndPoint().y > right.getLeftEndPoint().y;
    // left.left, left.right, right.left colinear
    if(Point2D::colinear(left.getLeftEndPoint(),
			 left.getRightEndPoint(),
			 right.getLeftEndPoint()))
      return Point2D::rightTurn(left.getLeftEndPoint(),
				left.getRightEndPoint(),
				right.getRightEndPoint());
    // normal case
    return Point2D::rightTurn(left.getLeftEndPoint(),
			      left.getRightEndPoint(),
			      right.getLeftEndPoint());
  }

  // returns true if left belongs above right in ascending order
  bool LineSegment::yasc(const LineSegment& left, const LineSegment& right) {
    // both vertical
    if(left.isVertical() && right.isVertical())
      return left.getBottomEndPoint().y < right.getBottomEndPoint().y;
    // left is vertical
    if(left.isVertical())
      return left.getBottomEndPoint().y < right.getLeftEndPoint().y;
    // left.left, left.right, right.left colinear
    if(Point2D::colinear(left.getLeftEndPoint(),
			 left.getRightEndPoint(),
			 right.getLeftEndPoint()))
      return Point2D::leftTurn(left.getLeftEndPoint(),
			       left.getRightEndPoint(),
			       right.getRightEndPoint());
    // normal case
    return Point2D::leftTurn(left.getLeftEndPoint(),
			     left.getRightEndPoint(),
			     right.getLeftEndPoint());
  }
  
  bool LineSegment::xdesc(const LineSegment& bottom, const LineSegment& top) {
    // both horizontal
    if(bottom.isHorizontal() && top.isHorizontal())
      return bottom.getLeftEndPoint().x > top.getLeftEndPoint().x;
    // left is vertical
    if(bottom.isHorizontal())
      return bottom.getLeftEndPoint().x > top.getBottomEndPoint().x;
    // left.left, left.right, right.left colinear
    if(Point2D::colinear(bottom.getBottomEndPoint(),
			 bottom.getTopEndPoint(),
			 top.getBottomEndPoint()))
      return Point2D::rightTurn(bottom.getBottomEndPoint(),
				bottom.getTopEndPoint(),
				top.getTopEndPoint());
    // normal case
    return Point2D::rightTurn(bottom.getBottomEndPoint(),
			      bottom.getTopEndPoint(),
			      top.getBottomEndPoint());
  }

  bool LineSegment::xasc(const LineSegment& bottom, const LineSegment& top) {
    // both horizontal
    if(bottom.isHorizontal() && top.isHorizontal())
      return bottom.getLeftEndPoint().x < top.getLeftEndPoint().x;
    // left is vertical
    if(bottom.isHorizontal())
      return bottom.getLeftEndPoint().x < top.getBottomEndPoint().x;
    // left.left, left.right, right.left colinear
    if(Point2D::colinear(bottom.getBottomEndPoint(),
			 bottom.getTopEndPoint(),
			 top.getBottomEndPoint()))
      return Point2D::leftTurn(bottom.getBottomEndPoint(),
			       bottom.getTopEndPoint(),
			       top.getTopEndPoint());
    // normal case
    return Point2D::leftTurn(bottom.getBottomEndPoint(),
			     bottom.getTopEndPoint(),
			     top.getBottomEndPoint());
  }

  ostream& operator<<(ostream& os, const LineSegment& ls) {
    return os << ls.getFirstEndPoint() << " " << ls.getSecondEndPoint();
  }
  
  istream& operator>>(istream& is, LineSegment& ls) {
    is >> ls.first >> ls.second;
    ls.build();
    return is;
  }

  void LineSegment::build(void) {
    if(first.x < second.x) {
      left = first;
      right = second;
    } else {
      left = second;
      right = first;
    }
    if(first.y < second.y) {
      bottom = first;
      top = second;
    } else {
      bottom = second;
      top = first;
    }
  }

  bool LineSegment::operator==(const LineSegment& other) const {
    return ((first == other.first) && (second == other.second))
      || ((first == other.second) && (second == other.first));
  }

  bool LineSegment::operator!=(const LineSegment& other) const {
    return !((*this) == other);
  }
}
