#include "TestString.hpp"

void TestString::TestAll()
{
    Test1();
}

void TestString::Test1()
{
    Vector<String> strs = {"aaa", "bbb", "ccc"};
    bool pass = String::Join(",", strs.begin(), strs.end()) == String("aaa,bbb,ccc");
    std::cout << __FUNCTION__ << "  " << (pass ? "pass" : "fail") << std::endl;
}
