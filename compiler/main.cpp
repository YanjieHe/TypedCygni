#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

extern FILE *yyin;
extern Expression* parsedTree;
// extern FILE *yyout;
// extern int yyparse(void);
// #include "ConsoleApp.hpp"
// #include "Lexer.hpp"
// #include "Parser.hpp"
// #include "Visitor.hpp"
using namespace std;
// extern FILE *yyin;

int main(int argc, char **argv) {
  cout << "Hello Cygni!" << endl;
  yyin = fopen("programs/simple_expression.cyg", "r+");
  if (yyin == NULL) {
    printf("code file missing\n");
  } else {
    cout << "start parsing" << endl;
    yyparse();
    fclose(yyin);
    AstJsonSerializer serializer;
    json jsonObj = serializer.VisitExpression(parsedTree);
    std::ofstream output("programs/output/simple_expression.json");
    output << std::setw(4) << jsonObj << std::endl;
  }
  cout << "Press any key to close..." << endl;
  cin.get();
  return 0;
}
