#include <iostream>
#include <vector>
#include <linq/Linq.hpp>
#include "Expression.hpp"
#include <iomanip>

using std::cout;
using std::endl;
using std::vector;
using std::make_shared;

using namespace linq;

int main(int argc, char **argv) {
  cout << "hello world!" << endl;
  vector<int> ints = {9, 8, 7, 6, 5};
  auto res = from(ints)
                 .select([](int x) -> int { return x * 3; })
                 .where([](int x) -> bool { return x % 2 == 1; })
                 .take(2);
  auto node =
      make_shared<ConstantExpression>(Position(), ExpressionType::INT, "123");
  auto type = make_shared<FunctionType>(
      vector<Type::Ptr>{Type::Int(), Type::Double()}, Type::String());

  cout << std::setw(4) << node->ToJson() << endl;
  cout << std::setw(4) << type->ToJson() << endl;
  vector<int> list1 = {9, 8, 7, 6, 5};
  vector<int> list2 = {9, 8, 7, 6, 5};
  vector<int> list3 = {9, 4, 7, 6, 5};
  vector<int> list4 = {9, 4, 7, 6};
  cout << std::boolalpha
       << from(list1).sequence_equal(list2, [](int x, int y) { return x == y; })
       << endl;
  cout << std::boolalpha
       << from(list1).sequence_equal(list3, [](int x, int y) { return x == y; })
       << endl;
  cout << std::boolalpha
       << from(list1).sequence_equal(list4, [](int x, int y) { return x == y; })
       << endl;
  cout << "finished" << endl;
  return 0;
}