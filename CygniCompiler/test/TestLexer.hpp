#ifndef CYGNICOMPILER_TESTLEXER_HPP
#define CYGNICOMPILER_TESTLEXER_HPP

#include "../String.hpp"
#include "../Token.hpp"
#include "../Predef.hpp"

namespace TestLexer
{

    bool AssertTokensEqual(const std::vector<Token> &tokens1, const std::vector<Token> &tokens2);

    void DisplayTokens(const Vector<Token> &tokens);

    void Test1();

    void Test2();

    void TestAll();

};

#endif //CYGNICOMPILER_TESTLEXER_HPP
