///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    LineSegment.hpp                                                  //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// PURPOSE: Stores the information associated with a line segment in         //
//          R2.  In other words, a pair of end points of the line segment.   //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////
#ifndef LINESEGMENT_HPP
#define LINESEGMENT_HPP

#include "Point2D.hpp"
#include <ostream>

namespace geometry {

  struct IntersectionResult {
    bool isParallel;
    bool isCoincident;
    bool isIntersecting;
    Point2D point;
    IntersectionResult() :
      isParallel(false),
      isCoincident(false),
      isIntersecting(false)
    {}
  };
  
  class LineSegment {
  public:
    friend istream& operator>>(istream&,LineSegment&);
    LineSegment(int ax=0, int ay=0, int bx=0, int by=0);
    LineSegment(Point2D&,Point2D&);
    LineSegment(const Point2D&,const Point2D&);
    ~LineSegment();

    const Point2D& getFirstEndPoint() const;
    const Point2D& getSecondEndPoint() const;

    const Point2D& getLeftEndPoint() const;
    const Point2D& getRightEndPoint() const;
    const Point2D& getTopEndPoint() const;
    const Point2D& getBottomEndPoint() const;
    
    IntersectionResult intersection(const LineSegment&) const;

    bool operator<(const LineSegment&) const;
    bool operator>(const LineSegment&) const;

    static bool ydesc(const LineSegment&,const LineSegment&);
    static bool yasc(const LineSegment&,const LineSegment&);
    static bool xdesc(const LineSegment&,const LineSegment&);
    static bool xasc(const LineSegment&,const LineSegment&);

    bool operator==(const LineSegment&) const;

  private:
    Point2D first, second;
  };

  ostream& operator<<(ostream&,const LineSegment&);
    
}

#endif
