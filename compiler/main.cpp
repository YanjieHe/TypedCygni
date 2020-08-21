#include "ConsoleApp.hpp"
#include "Expression.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Visitor.hpp"

using namespace std;

int main(int argc, char **argv) {
  using namespace cygni;
  cout << "Hello Cygni!" << endl;
  cout << "Press any key to close..." << endl;
  cin.get();
  return 0;
}
