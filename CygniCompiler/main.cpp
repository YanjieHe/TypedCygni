#include <iostream>
#include <memory>
#include "Type.hpp"
#include "Predef.hpp"
#include "String.hpp"
#include "test/TestLexer.hpp"
#include "test/TestParser.hpp"
#include "test/TestString.hpp"
#include "test/TestEndian.hpp"
#include "test/TestTypeChecker.hpp"

using namespace std;

int main(int argc, char **argv)
{
    cout << "Welcome!" << endl;
    TestString::TestAll();
    TestEndian::TestAll();
    TestLexer::TestAll();
    TestParser::TestAll();
    TestTypeChecker::TestAll();
    return 0;
}