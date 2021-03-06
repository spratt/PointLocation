///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_polygonal_subdivision.cpp                                   //
//                                                                           //
// MODULE:  Polygonal Subdivision                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iterator>
#include <ctime>
#include "../Point2D.hpp"
#include "../LineSegment.hpp"
#include "../PolygonalSubdivision.hpp"

using namespace std;
using namespace geometry;

int main(int argc, char** argv) {
  // if not enough parameters provided, print a helpful message and quit
  if(argc < 3) {
    cerr << "usage: " << argv[0] << " [segments file] [points file]" << endl
	 << "\t where [segments file] is a file containing line segments" << endl
	 << "\t and   [points file]   is a file containing query points" << endl;
    return 0;
  }
  time_t start = time(0);
  time_t last = start;
  // for segment input
  ifstream segment_file(argv[1]);
  istream_iterator<LineSegment> segment_begin(segment_file);
  istream_iterator<LineSegment> segment_end;
  
  // for point input
  ifstream point_file(argv[2]);
  istream_iterator<Point2D> point_begin(point_file);
  istream_iterator<Point2D> point_end;
  
  PolygonalSubdivision ps;

  // read in the segments
  while(segment_begin != segment_end) {
    try{
      ps.addLineSegment(*segment_begin);
    } catch(char const* str) {
      cerr << "=== ERROR=== " << str <<endl;
      return 1;
    }
    ++segment_begin;
  }
  // ready the structure for queries
  try {
    ps.lock();
  } catch(string str) {
    cerr << "=== ERROR=== " << str << endl;
    return 2;
  }

  time_t now = time(0);
  cerr << "Build took: " << difftime(now,last) << endl;
  last = now;

  // locate and print the containing polygon for each point
  while(point_begin != point_end) {
    try{
      cout << "(" << *point_begin << "): ";
      QueryResult result = ps.locate_point(*point_begin);
      if(result.outer)
	cout << "outer" << endl;
      else if(result.vertex)
	cout << "Vertex" << endl;
      else if(result.edge)
	cout << "Edge: (" << result.above << ")" << endl;
      else
	cout << "(" << result.above << ") (" << result.below << ")" << endl;
    }catch(char const* str) {
      cerr << "=== ERROR === " << str << endl;
      return 3;
    }
    ++point_begin;
  }

  now = time(0);
  cerr << "Queries took: " << difftime(now,last) << endl;
  cerr << "Total time: " << difftime(now,start) << endl;
  last = now;
  
  return 0;
}
