#include "dsa/BinaryTree.h"
#include "dsa/Queue.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
int main() {
  BinaryTree<int> b{};
  b.insert(5);
  b.insert(10);
  b.insert(4);
  b.insert(3);
  b.insert(2);
  b.erase(2);
  cout << b.depth() << endl;

  return 0;
}
