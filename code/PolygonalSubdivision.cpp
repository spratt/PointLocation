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

// DEBUG
#include <iostream>
using namespace std;

namespace geometry {

  PolygonalSubdivision::PolygonalSubdivision()
    : line_segments(), points(), sweep_points(), psl(), _locked(false)
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  void PolygonalSubdivision::addLineSegment(LineSegment& ls) {
    line_segments.push_back(ls);
    points.insert(ls.getFirstEndPoint());
    points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(const LineSegment& ls) {
    line_segments.push_back(ls);
    points.insert(ls.getFirstEndPoint());
    points.insert(ls.getSecondEndPoint());
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
    sort(line_segments.begin(),line_segments.end(),LineSegment::xdesc);

    for(set<Point2D>::iterator point = points.begin();
	point != points.end();
	++point) {
      // add points whose left end points are on the sweep line
      {
	LineSegment& line = line_segments.back();
	while(line_segments.size() > 0 &&
	      line.getLeftEndPoint().x <= (*point).x) {
	  psl.insert(line);
	  line_segments.pop_back();
	  line = line_segments.back();
	}
      }
      // remove points whose right end points are on the sweep line
      int present = psl.getPresent();
      PSLIterator<LineSegment> line = psl.begin(present);
      while(line != psl.end(present)) {
	if((*line).getRightEndPoint().x == (*point).x)
	  line.remove();
	else
	  ++line;
      }
      psl.incTime();
      sweep_points.push_back(*point);
    }
  }
  const LineSegment PolygonalSubdivision::locate_point(const Point2D& p) {
    // basic error checking
    if(!_locked)
      throw "PolygonalSubdivision must be locked before use";
    if(psl.empty(0))
      throw "No line segments";

    int index = int(lower_bound(sweep_points.begin(),
				sweep_points.end(),
				p)
		    - sweep_points.begin());

    if(p.x != (sweep_points[index]).x && index > 0)
      --index;
    
    return *(psl.find(LineSegment(p,p),index));
  }

}
