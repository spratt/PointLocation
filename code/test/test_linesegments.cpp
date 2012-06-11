#include <iostream>
#include "../lib/PersistentSkipList/PersistentSkipList.hpp"
#include "../LineSegment.hpp"

using namespace std;
using namespace geometry;

int main(int argc, char** argv) {
  PersistentSkipList<LineSegment> psl;
  
  psl.insert(LineSegment(0,0,2,0));
  psl.insert(LineSegment(0,1,2,5));
  psl.insert(LineSegment(0,2,2,6));
  psl.insert(LineSegment(0,6,2,7));
  
  int present = psl.getPresent();
  PSLIterator<LineSegment> it = psl.begin(present,0);
  while(it != psl.end(present)) {
    cout << *it << endl;
    ++it;
  }
  return 0;
}
