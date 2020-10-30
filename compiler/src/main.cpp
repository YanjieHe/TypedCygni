#include <iostream>
#include <vector>
#include "Linq.hpp"

using std::cout;
using std::endl;
using std::vector;
using namespace linq;

int main(int argc, char **argv) {
  cout << "hello world!" << endl;
  vector<int> ints = {9, 8, 7, 6, 5};
  auto res = from(ints)
                 .select([](int x) -> int { return x * 3; })
                 .where([](int x) -> bool { return x % 2 == 1; })
                 .take(2);
  for (int i : res) {
    cout << i << endl;
  }
  cout << "finished" << endl;
  return 0;
}