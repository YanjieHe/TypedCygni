#include "Parser.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>


extern FILE *yyin;
extern Statement *parsedTree;
extern vector<SyntaxError> syntaxErrorList;
extern string currentInputSourcePath;
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
  const char* path = "programs/simple_expression.cyg";
  currentInputSourcePath = path;
  yyin = fopen(path, "r+");
  if (yyin == NULL) {
    printf("code file missing\n");
  } else {
    // cout << "start parsing" << endl;
    yyparse();
    fclose(yyin);
    for (auto syntaxError : syntaxErrorList) {
      cout << syntaxError.ToString() << endl;
    }
    AstJsonSerializer serializer;
    json jsonObj = serializer.VisitStatement(parsedTree);
    std::ofstream output("programs/output/simple_expression.json");
    output << std::setw(4) << jsonObj << std::endl;
  }
  cout << "Press any key to close..." << endl;
  cin.get();
  return 0;
}
