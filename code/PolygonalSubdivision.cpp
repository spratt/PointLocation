
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
    : line_segments_left(),
      line_segments_right(),
      points(),
      sweep_points(),
      psl(),
      _locked(false)
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  void PolygonalSubdivision::addLineSegment(LineSegment& ls) {
    line_segments_left.push_back(ls);
    line_segments_right.push_back(ls);
    points.insert(ls.getFirstEndPoint());
    points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(const LineSegment& ls) {
    line_segments_left.push_back(ls);
    line_segments_right.push_back(ls);
    points.insert(ls.getFirstEndPoint());
    points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  bool leftDescX(const LineSegment& a, const LineSegment& b) {
    return a.getLeftEndPoint().x > b.getLeftEndPoint().x;
  }

  bool rightDescX(const LineSegment& a, const LineSegment& b) {
    return a.getRightEndPoint().x > b.getRightEndPoint().x;
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
    sort(line_segments_left.begin(),line_segments_left.end(),leftDescX);
    sort(line_segments_right.begin(),line_segments_right.end(),rightDescX);

    for(set<Point2D>::iterator point = points.begin();
	point != points.end();
	++point) {
      // add points whose left end points are on the sweep line
      {
	LineSegment& line = line_segments_left.back();
	while(line_segments_left.size() > 0 &&
	      line.getLeftEndPoint().x <= (*point).x) {
	  psl.insert(line);
	  line_segments_left.pop_back();
	  line = line_segments_left.back();
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
  
  QueryResult PolygonalSubdivision::locate_point(const Point2D& p) {
    // basic error checking
    if(!_locked)
      throw "PolygonalSubdivision must be locked before use";
    if(psl.empty(0))
      throw "No line segments";

    unsigned int index = int(lower_bound(sweep_points.begin(),
					 sweep_points.end(),
					 p)
			     - sweep_points.begin());

    if(p.x != (sweep_points[index]).x && index > 0)
      --index;

    // DEBUG
    cout << endl;
    cout << "===DEBUG=== Index:   " << index << endl;
    cout << "===DEBUG=== Before:  " << sweep_points[index] << endl;
    if(sweep_points.size() > index+1)
      cout << "===DEBUG=== After:   " << sweep_points[index+1] << endl;
    // END DEBUG

    PSLIterator<LineSegment> it = psl.find(LineSegment(p,p),index);
    LineSegment above = *it;
    ++it;
    LineSegment below = *it;

    // DEBUG
    cout << "===DEBUG=== Above:   " << above << endl;
    cout << "===DEBUG=== Below:   " << below << endl;
    ++it;
    cout << "===DEBUG=== Below2:  " << *it << endl;
    ++it;
    cout << "===DEBUG=== Below3:  " << *it << endl;
    // END DEBUG

    bool outer = below == LineSegment(0,0,0,0)
      || above == LineSegment(0,0,0,0);
    
    return QueryResult(above,below,outer);
  }

}
