///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    Point2D.hpp                                                      //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// PURPOSE: Stores the information associated with a point in R2.  In        //
//          other words a pair of coordinates                                //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// coord_t Point2D.x                    the x coordinate of a point          //
// coord_t Point2D.y                    the y coordinate of a point          //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////
#ifndef POINT2D_HPP
#define POINT2D_HPP

#include <fstream>
#include <limits>

using namespace std;

#include <LEDA/numbers/rational.h>

using leda::rational;

namespace geometry {
  // Defines the implementation and precision of coordinates
  typedef rational coord_t;
  
  /////////////////////////////////////////////////////////////////////////////
  // Point2D interface                                                       //
  /////////////////////////////////////////////////////////////////////////////
  class Point2D {
  public:
    coord_t x, y;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: Point2D                                                //
    //                                                                       //
    // PURPOSE:       Empty constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2D()
      : x(0), y(0)
    {}
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: Point2D                                                //
    //                                                                       //
    // PURPOSE:       Basic constructor.                                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   coord_t/x                                              //
    //   Description: The x coordinate of a point in 2d.                     //
    //                                                                       //
    //   Type/Name:   coord_t/y                                              //
    //   Description: The y coordinate of a point in 2d.                     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2D(coord_t x, coord_t y)
      : x(x), y(y)
    {}

    Point2D(const Point2D& other)
      : x(other.x), y(other.y)
    {}

    Point2D operator-(const Point2D& other) const;

    static coord_t crossProduct(const Point2D& a, const Point2D& b);

    static bool leftTurn(const Point2D& a, const Point2D& b, const Point2D& c);

    static bool rightTurn(const Point2D& a, const Point2D& b, const Point2D& c);

    static bool colinear(const Point2D& a, const Point2D& b, const Point2D& c);
    
    struct yxasc {
      bool operator()(const Point2D& a, const Point2D& b) const {
	if(a.y == b.y)
	  return a.x < b.x;
	return a.y < b.y;
      }
    };

    struct yxdesc {
      bool operator()(const Point2D& a, const Point2D& b) const {
	if(a.y == b.y)
	  return a.x > b.x;
	return a.y > b.y;
      }
    };
  };

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator>                                                //
  //                                                                         //
  // PURPOSE:       Compares two Point2D objects based on their x coordinate.//
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   Point2D/a                                                //
  //   Description: The first point to compare.                              //
  //                                                                         //
  //   Type/Name:   Point2D/b                                                //
  //   Description: The second point to compare.                             //
  //                                                                         //
  // RETURN:        True if a is greater than b, false otherwise.            //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  bool operator>(const Point2D& a, const Point2D& b);
  bool operator>=(const Point2D& a, const Point2D& b);

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator<                                                //
  //                                                                         //
  // PURPOSE:       Compares two Point2D objects based on their x coordinate.//
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   Point2D/a                                                //
  //   Description: The first point to compare.                              //
  //                                                                         //
  //   Type/Name:   Point2D/b                                                //
  //   Description: The second point to compare.                             //
  //                                                                         //
  // RETURN:        True if a is less than b, false otherwise.               //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  bool operator<(const Point2D& a, const Point2D& b);
  bool operator<=(const Point2D& a, const Point2D& b);

  bool operator==(const Point2D& a, const Point2D& b);

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator<<                                               //
  //                                                                         //
  // PURPOSE:       Formats a Point2D nicely for an ostream.                 //
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   ostream/os                                               //
  //   Description: An ostream to which to send the Point2D.                 //
  //                                                                         //
  //   Type/Name:   Point2D/p                                                //
  //   Description: The Point2D to format and send.                          //
  //                                                                         //
  // RETURN:        The ostream input.                                       //
  //                This is standard practice for chaining such as:          //
  //                cout << "Point: " << point << endl;                      //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  ostream& operator<<(ostream& os, const Point2D& p);

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator>>                                               //
  //                                                                         //
  // PURPOSE:       Reads in a point from an istream.                        //
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   istream/os                                               //
  //   Description: An istream from which to read the Point2D.               //
  //                                                                         //
  //   Type/Name:   Point2D/p                                                //
  //   Description: The Point2D in which to store the read point.            //
  //                                                                         //
  // RETURN:        The istream input.                                       //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  istream& operator>>(istream& os, Point2D& p);
}
namespace std {
  /////////////////////////////////////////////////////////////////////////////
  // Numeric Limits Interface                                                //
  /////////////////////////////////////////////////////////////////////////////
  template <>
  class numeric_limits<geometry::Point2D> {
    static const bool is_specialized = true;
    static const bool has_infinity =
      numeric_limits<geometry::coord_t>::has_infinity;
    static geometry::Point2D infinity();
    static geometry::Point2D max();
    static geometry::Point2D min();
  };
}
#endif
