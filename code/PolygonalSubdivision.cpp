
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.cpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "PolygonalSubdivision.hpp"
#include <iostream>

namespace geometry {

  PolygonalSubdivision::PolygonalSubdivision()
    : line_segments_left(),
      line_segments_right(),
      x_coords(),
      sweep_points(),
      psl(),
      _locked(false),
#ifdef NDEBUG
      _log(clog,"/dev/null") // unportable hack
#else
      _log(clog,"log_PS.txt")
#endif
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  void PolygonalSubdivision::addLineSegment(LineSegment& ls) {
    line_segments_left.push_back(ls);
    x_coords.insert(ls.getFirstEndPoint().x);
    x_coords.insert(ls.getSecondEndPoint().x);
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(const LineSegment& ls) {
    line_segments_left.push_back(ls);
    x_coords.insert(ls.getFirstEndPoint().x);
    x_coords.insert(ls.getSecondEndPoint().x);
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

    for(set< coord_t >::iterator coord = x_coords.begin();
	coord != x_coords.end();
	++coord) {
      // add points whose left end points are on the sweep line
      {
	LineSegment& line = line_segments_left.back();
	while(line_segments_left.size() > 0 &&
	      line.getLeftEndPoint().x <= (*coord)) {
	  if(line.isVertical()) {
	    if(vertical_lines.count(line.getFirstEndPoint().x) == 0)
	      vertical_lines[line.getFirstEndPoint().x] =
		vector<LineSegment>();
	    vertical_lines[line.getFirstEndPoint().x].push_back(line);
	    line_segments_left.pop_back();
	    line = line_segments_left.back();
	    continue;
	  }
	  try {
	    psl.insert(line);
	  } catch(char const* exception) {
	    psl.drawPresent();
	    throw "Error while trying to insert line";
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
	vector<LineSegment>::iterator it =
	  line_segments_right[*coord].begin();
	vector<LineSegment>::iterator end = 
	  line_segments_right[*coord].end();
	while(it != end) {
	  PSLIterator<LineSegment> toRemove = psl.find((*it),present);
	  if((*it) != (*toRemove)) {
	    clog << "=== ERROR ===" << endl
		 << "Deletion mismatch" << endl
		 << "Sought: " << (*it) << endl
		 << "Found:  " << (*toRemove) << endl;
	  }
	  assert((*it) == (*toRemove));
	  toRemove.remove();
	  ++it;
	}
	line_segments_right.erase(*coord);
      }
      
      psl.incTime();
      sweep_points.push_back(*coord);
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
					 p.x)
			     - sweep_points.begin());

    if(p.x != sweep_points[index] && index > 0)
      --index;

    // check if left of first sweep line
    if(index == 0 && p.x < sweep_points[index])
      return QueryResult(LineSegment(0,0),
			 LineSegment(0,0),
			 true); // outer

    LineSegment toFind(p,p);
    
    PSLIterator<LineSegment> it = psl.find(toFind,index);
    LineSegment above = *it;
    ++it;
    LineSegment below = *it;

    // check if query point was on sweep line
    if(p.x == sweep_points[index]) {
      // check if query point is on a vertical line
      for(vector<LineSegment>::iterator it = vertical_lines[p.x].begin();
	  it != vertical_lines[p.x].end();
	  ++it) {
	if(p.y < (*it).getTopEndPoint().y && p.y > (*it).getBottomEndPoint().y)
	  return QueryResult(*it,
			     *it,
			     false, // outer
			     false, // vertex
			     true); // edge
      }

      // check if query point is on a vertex
      if(p == above.getFirstEndPoint() ||
	 p == above.getSecondEndPoint())
	return QueryResult(above,
			   above,
			   false, // outer
			   true); // vertex
      if(p == below.getFirstEndPoint() ||
	 p == below.getSecondEndPoint())
	return QueryResult(below,
			   below,
			   false, // outer
			   true); // vertex

      // otherwise, point must be on a face, so proceed normally
    }

    // check if query point was on the above line
    if(toFind <= above && toFind >= above) {
      return QueryResult(above,
			 above,
			 false, // outer
			 false, // vertex
			 true); // edge
    } else if(toFind <= below && toFind >= below) {
      return QueryResult(below,
			 below,
			 false, // outer
			 false, // vertex
			 true); // edge
    }

    bool outer =
	below == LineSegment(0,0,0,0) ||
	above == LineSegment(0,0,0,0);
    
    return QueryResult(above,below,outer);
  }

}
