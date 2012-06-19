
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
    points.insert(ls.getFirstEndPoint());
    points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(const LineSegment& ls) {
    line_segments_left.push_back(ls);
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

    for(set<Point2D>::iterator point = points.begin();
	point != points.end();
	++point) {
      // add points whose left end points are on the sweep line
      {
	LineSegment& line = line_segments_left.back();
	while(line_segments_left.size() > 0 &&
	      line.getLeftEndPoint().x <= (*point).x) {
	  try {
	    psl.insert(line);
	  } catch(char const* exception) {
	    cout << "=== EXCEPTION === " << exception << endl
		 << "=== EXCEPTION === " << line << " already exists" << endl;
	    psl.drawPresent();
	    throw "BAILING";
	  }
	  if(line_segments_right.count(line.getRightEndPoint().x) == 0) {
	    line_segments_right[line.getRightEndPoint().x] =
	      vector<LineSegment>();
	  }
	  line_segments_right[line.getRightEndPoint().x].push_back(line);
	  line_segments_left.pop_back();
	  line = line_segments_left.back();
	}
      }
      // remove points whose right end points are at most the sweep line
      int present = psl.getPresent();
      {
	//////////////////////////////////////////////////////////////////////////////
	// DEBUG DEBUG Print current state of psl                                   //
	//////////////////////////////////////////////////////////////////////////////

	PSLIterator<LineSegment> it = psl.begin(present);
	cout << "PSL(" << present << "): " << *it;
	while(++it != psl.end(present)) {
	  cout << ", " << *it;
	}
	cout << endl;
	//////////////////////////////////////////////////////////////////////////////
	// DEBUG DEBUG                                                              //
	//////////////////////////////////////////////////////////////////////////////
      }
      {
	vector<LineSegment>::iterator it =
	  line_segments_right[(*point).x].begin();
	vector<LineSegment>::iterator end = 
	  line_segments_right[(*point).x].end();
	while(it != end) {
	  cout << "=== DEBUG === " << "deleting " << (*it) << "...";
	  PSLIterator<LineSegment> toRemove = psl.find((*it),present);
	  assert((*it) == (*toRemove));
	  toRemove.remove();
	  cout << "deleted." << endl;
	  ++it;
	}
	line_segments_right.erase((*point).x);
	cout << "=== DEBUG === " << "done deletions." << endl;
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
    
    PSLIterator<LineSegment> it = psl.find(LineSegment(p,p),index);
    LineSegment above = *it;
    ++it;
    LineSegment below = *it;

    bool outer = below == LineSegment(0,0,0,0)
      || above == LineSegment(0,0,0,0);
    
    return QueryResult(above,below,outer);
  }

}
