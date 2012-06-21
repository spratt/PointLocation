#include <vector>
#include <iostream>
#include "../lib/PersistentSkipList/PersistentSkipList.hpp"
#include "../Point2D.hpp"
#include "../LineSegment.hpp"

using namespace std;
using namespace geometry;

void printBar() {
  cout << "======================================================================"
       << endl;
}

int main(int argc, char** argv) {
  PersistentSkipList<LineSegment> psl;
  /////////////////////////////////////////////////////////////////////////////
  // Test intersection calculations                                          //
  /////////////////////////////////////////////////////////////////////////////

  // The following doesn't work if coord_t is int
  // LineSegment line_a(0,0,2,2);
  // LineSegment line_b(0,2,2,0);
  // IntersectionResult result = line_a.intersection(line_b);
  // cout << "Intersection of " << line_a << " with " << line_b
  //      << " gives: " << result.point << endl;
  
  // assert(result.isParallel == false);
  // assert(result.isCoincident == false);
  // assert(result.isIntersecting == true);
  // assert(result.point == Point2D(1,1));

  /////////////////////////////////////////////////////////////////////////////
  // Test precedence                                                         //
  /////////////////////////////////////////////////////////////////////////////
  LineSegment ab(0,0,2,3);
  LineSegment ac(0,0,5,0);
  LineSegment bc(2,3,5,0);

  assert(ab < ac);
  assert(bc < ac);

  Point2D q0(1,2);
  Point2D q1(1,1);
  Point2D q2(3,1);
  Point2D q3(4,2);

  assert(LineSegment(q0,q0)<ab);
  assert(!(LineSegment(q0,q0)>ab));
  assert(LineSegment(q1,q1)>ab);
  assert(LineSegment(q1,q1)<ac);
  assert(!(LineSegment(q1,q1)>ac));
  assert(LineSegment(q2,q2)>bc);
  assert(LineSegment(q2,q2)<ac);
  assert(LineSegment(q3,q3)<bc);
  assert(!(LineSegment(q2,q2)>ac));
  assert(!(LineSegment(q3,q3)>bc));

  cout << "Building psl..." << endl;
  cout << "\t ...inserting " << ab << endl;
  psl.insert(ab);
  cout << "\t ...inserting " << ac << endl;
  psl.insert(ac);
  psl.incTime();
  int present = psl.getPresent();
  cout << "\t ...deleting " << ab << endl;
  psl.drawPresent();
  psl.find(ab,present).remove();
  psl.drawPresent();
  cout << "\t ...inserting " << bc << endl;
  psl.insert(bc);
  cout << "done." << endl;

  {
    printBar();
    cout << "Drawing all times of skiplist..." << endl;
    for(int i = 0; i <= psl.getPresent(); ++i) {
      psl.draw(i);
      printBar();
    }
  }

  {
    vector<Point2D> query_points;
    vector<int> times;
    query_points.push_back(q0); times.push_back(0);
    query_points.push_back(q1); times.push_back(0);
    query_points.push_back(q2); times.push_back(1);
    query_points.push_back(q3); times.push_back(1);

    for(unsigned int i = 0; i < times.size(); ++i) {
      Point2D point = query_points[i];
      int time = times[i];
      cout << "Searching for " << point << " at time " << time << ":" <<endl;
      PSLIterator<LineSegment> it = psl.find(LineSegment(point,point),time);
      while(it != psl.end(time)) {
	cout << *it << " ";
	++it;
      }
      cout << endl;
    }
  }

  return 0;
}
