///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.cpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "PolygonalSubdivision.hpp"
#include <algorithm>

namespace geometry {

  PolygonalSubdivision::PolygonalSubdivision()
    : line_segments(), sweep_points(), _locked(false)
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  void PolygonalSubdivision::addLineSegment(LineSegment& ls) {
    line_segments.push_back(ls);
    sweep_points.insert(ls.getFirstEndPoint());
    sweep_points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(const LineSegment& ls) {
    line_segments.push_back(ls);
    sweep_points.insert(ls.getFirstEndPoint());
    sweep_points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::lock() {
    if(_locked)
      return;

    // lock the division
    _locked = true;

    // build the structure

    ///////////////////////////////////////////////////////////////////////////
    // Each time stores the line segments which span from one sweep
    // point to the next.
    // 
    // All line segments not in the structure whose leftmost end points
    // lie on the sweep point should be added to the structure
    //
    // - store segments on a stack in increasing order of left end
    //   point
    //
    // All line segments in the structure whose right most end points
    // lie on the sweep point should be removed from the structure
    ///////////////////////////////////////////////////////////////////////////
    sort(line_segments.begin(),line_segments.end(),LineSegment::xasc);
  }
  LineSegment& PolygonalSubdivision::locate_point(Point2D& p) {
    // basic error checking
    if(!_locked)
      throw "PolygonalSubdivision must be locked before use";
    if(line_segments.empty())
      throw "No line segments";
    
    return line_segments[0];
  }

}
