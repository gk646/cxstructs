#include "dsa/BinaryTree.h"
#include "dsa/Queue.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dsa/HashMap.h"


using namespace std;



int main() {
  HashMap<string, int> hashMap;
  
  hashMap.put("hey", 3);
  
  std::cout<< hashMap["hey"] << std::endl;
  
  return 0;
}
