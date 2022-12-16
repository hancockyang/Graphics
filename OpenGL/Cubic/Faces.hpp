#pragma once

#include <vector>

namespace Faces 
{
//Face to vertex mapping matrix
std::vector<std::vector<int>> faces{
       std::vector<int>{0,2,1},
       std::vector<int>{1,2,3},
       std::vector<int>{1,3,7},
       std::vector<int>{7,5,1},
       std::vector<int>{0,1,5},
       std::vector<int>{5,4,0},
       std::vector<int>{0,4,2},
       std::vector<int>{4,6,2},
       std::vector<int>{4,5,7},
       std::vector<int>{7,6,4},
       std::vector<int>{6,7,3},
       std::vector<int>{3,2,6}
   };
}
