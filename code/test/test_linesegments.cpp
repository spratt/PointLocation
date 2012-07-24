#include <vector>
#include <algorithm>
#include <iostream>
#include "../lib/PersistentSkipList/PersistentSkipList.hpp"
#include "../Point2D.hpp"
#include "../LineSegment.hpp"

using namespace std;
using namespace geometry;

bool leftDescX(const LineSegment& a, const LineSegment& b) {
  return a.getLeftEndPoint().x > b.getLeftEndPoint().x;
}

void printBar() {
  cout << "======================================================================"
       << endl;
}

int main(int argc, char** argv) {
  /////////////////////////////////////////////////////////////////////////////
  // Test getters                                                            //
  /////////////////////////////////////////////////////////////////////////////

  LineSegment seg1(1,2,3,4);
  assert(seg1.getLeftEndPoint().x == 1);
  assert(seg1.getRightEndPoint().x == 3);
  assert(seg1.getTopEndPoint().y == 4);
  assert(seg1.getBottomEndPoint().y == 2);

  LineSegment seg2(Point2D(1,2),Point2D(3,4));
  assert(seg2.getLeftEndPoint().x == 1);
  assert(seg2.getRightEndPoint().x == 3);
  assert(seg2.getTopEndPoint().y == 4);
  assert(seg2.getBottomEndPoint().y == 2);

  const Point2D p1(1,2);
  const Point2D p2(3,4);
  LineSegment seg3(p1,p2);
  assert(seg3.getLeftEndPoint().x == 1);
  assert(seg3.getRightEndPoint().x == 3);
  assert(seg3.getTopEndPoint().y == 4);
  assert(seg3.getBottomEndPoint().y == 2);

  
  LineSegment seg4(3,4,1,2);
  assert(seg4.getLeftEndPoint().x == 1);
  assert(seg4.getRightEndPoint().x == 3);
  assert(seg4.getTopEndPoint().y == 4);
  assert(seg4.getBottomEndPoint().y == 2);

  LineSegment seg5(Point2D(3,4),Point2D(1,2));
  assert(seg5.getLeftEndPoint().x == 1);
  assert(seg5.getRightEndPoint().x == 3);
  assert(seg5.getTopEndPoint().y == 4);
  assert(seg5.getBottomEndPoint().y == 2);

  const Point2D p3(3,4);
  const Point2D p4(1,2);
  LineSegment seg6(p3,p4);
  assert(seg6.getLeftEndPoint().x == 1);
  assert(seg6.getRightEndPoint().x == 3);
  assert(seg6.getTopEndPoint().y == 4);
  assert(seg6.getBottomEndPoint().y == 2);

  LineSegment seg7 = seg3;
  assert(seg7.getLeftEndPoint().x == 1);
  assert(seg7.getRightEndPoint().x == 3);
  assert(seg7.getTopEndPoint().y == 4);
  assert(seg7.getBottomEndPoint().y == 2);
  
  LineSegment seg8 = seg6;
  assert(seg8.getLeftEndPoint().x == 1);
  assert(seg8.getRightEndPoint().x == 3);
  assert(seg8.getTopEndPoint().y == 4);
  assert(seg8.getBottomEndPoint().y == 2);

  LineSegment seg9(seg3);
  assert(seg9.getLeftEndPoint().x == 1);
  assert(seg9.getRightEndPoint().x == 3);
  assert(seg9.getTopEndPoint().y == 4);
  assert(seg9.getBottomEndPoint().y == 2);

  LineSegment seg10(seg6);
  assert(seg10.getLeftEndPoint().x == 1);
  assert(seg10.getRightEndPoint().x == 3);
  assert(seg10.getTopEndPoint().y == 4);
  assert(seg10.getBottomEndPoint().y == 2);

  vector<LineSegment> lsVector;
  lsVector.push_back(seg1);
  lsVector.push_back(seg2);
  lsVector.push_back(seg3);
  lsVector.push_back(seg4);
  lsVector.push_back(seg5);
  lsVector.push_back(seg6);
  lsVector.push_back(seg7);
  lsVector.push_back(seg8);
  lsVector.push_back(seg9);
  lsVector.push_back(seg10);
  assert(lsVector.back().getLeftEndPoint().x == 1);
  sort(lsVector.begin(),lsVector.end(),leftDescX);
  assert(lsVector.back().getLeftEndPoint().x == 1);

  /////////////////////////////////////////////////////////////////////////////
  // Test other stuff                                                        //
  /////////////////////////////////////////////////////////////////////////////
  
  PersistentSkipList<LineSegment> psl;

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
