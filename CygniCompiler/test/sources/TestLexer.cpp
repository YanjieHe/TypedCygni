#include "TestLexer.hpp"
#include <iostream>
#include "Lexer.hpp"
#include "File.hpp"
#include <fstream>

using namespace std;

bool TestLexer::AssertTokensEqual(const std::vector<Token> &tokens1, const std::vector<Token> &tokens2)
{
    auto comparator = [](const Token &x, const Token &y) -> bool
    {
        return x.tag == y.tag && x.text == y.text;
    };
    return std::equal(tokens1.begin(), tokens1.end(), tokens2.begin(), tokens2.end(), comparator);
}

void TestLexer::Test1()
{
    bool pass = true;
    Lexer lexer("a + b * 2");
    std::vector<Token> tokens = lexer.ReadAll();
    auto t = [](Tag tag, const char *text)
    {
        return Token(0, 0, tag, text);
    };
    pass = pass & AssertTokensEqual(tokens, std::vector<Token>{
            t(Tag::Identifier, "a"),
            t(Tag::Add, "+"),
            t(Tag::Identifier, "b"),
            t(Tag::Multiply, "*"),
            t(Tag::Integer, "2"),
            t(Tag::Eof, "<EOF>")
    });
    cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << endl;
}

void TestLexer::TestAll()
{
    Test1();
//    Test2();
}

void TestLexer::Test2()
{
    Lexer lexer(File::ReadFile("test_code/code2.txt"));
    auto tokens = lexer.ReadAll();
    DisplayTokens(tokens);
}

void TestLexer::DisplayTokens(const Vector<Token> &tokens)
{
    for (const auto &token: tokens)
    {
        cout << token.line << ", " << token.column << ": ";
        cout << TagToString(token.tag) << " ";
        cout << token.text << endl;
    }
}
