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
  class PolygonalSubdivision {
  public:
    PolygonalSubdivision();
    ~PolygonalSubdivision();

    void addLineSegment(LineSegment&);
    void addLineSegment(const LineSegment&);

    void lock();
    LineSegment& locate_point(Point2D&);
    
  private:
    vector< LineSegment > line_segments;
    set< Point2D > sweep_points;
    PersistentSkipList< LineSegment > psl;
    
    bool _locked;
  };
}

#endif
