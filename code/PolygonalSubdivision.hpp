///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.hpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// PURPOSE: Solves the planar point location problem using a                 //
//          persistent skiplist.                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////

#ifndef POLYGONALSUBDIVISION_HPP
#define POLYGONALSUBDIVISION_HPP

#include <vector>
#include <set>
#include "Point2D.hpp"
#include "LineSegment.hpp"
#include "lib/PersistentSkipList/PersistentSkipList.hpp"

using namespace std;
using persistent_skip_list::PersistentSkipList;

namespace geometry {

  class QueryResult {
  public:
    bool outer;
    LineSegment above;
    LineSegment below;

    QueryResult(LineSegment a, LineSegment b, bool o=false)
      : outer(o), above(a), below(b)
    {}
  };
  
  class PolygonalSubdivision {
  public:
    PolygonalSubdivision();
    ~PolygonalSubdivision();

    void addLineSegment(LineSegment&);
    void addLineSegment(const LineSegment&);

    void lock();
    QueryResult locate_point(const Point2D&);
    
  private:
    vector< LineSegment > line_segments_left;
    map< int, LineSegment > line_segments_right;
    set< Point2D > points;
    vector< Point2D > sweep_points;
    PersistentSkipList< LineSegment > psl;
    
    bool _locked;
  };
  
}

#endif
