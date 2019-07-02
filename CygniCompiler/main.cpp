#include <iostream>
#include <memory>
#include "Type.hpp"
#include "Predef.hpp"
#include "String.hpp"
#include "TestLexer.hpp"
#include "TestParser.hpp"
#include "TestString.hpp"
#include "TestEndian.hpp"
#include "TestTypeChecker.hpp"
#include "TestLocator.hpp"
#include "TestCompiler.hpp"

using namespace std;

int main(int argc, char **argv)
{
    cout << "Welcome!" << endl;
    TestString::TestAll();
    TestEndian::TestAll();
    TestLexer::TestAll();
    TestParser::TestAll();
    TestTypeChecker::TestAll();
    TestLocator::TestAll();
    TestCompiler::TestAll();
    return 0;
}