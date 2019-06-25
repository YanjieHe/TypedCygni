#ifndef TEST_TYPE_CHECKER_HPP
#define TEST_TYPE_CHECKER_HPP

#include "../TypeChecker.hpp"

namespace TestTypeChecker
{
    void TestAll();

    Ptr<Scope> InitializeBasicTypes();

    void OutputTypeRecord(const std::string &path, const HashMap<int, Ptr<Type>> &typeRecord);

    void Test1();
}
#endif //TEST_TYPE_CHECKER_HPP
