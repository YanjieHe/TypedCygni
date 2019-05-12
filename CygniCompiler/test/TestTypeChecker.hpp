#ifndef TESTTYPECHECKER_HPP
#define TESTTYPECHECKER_HPP

#include "../TypeChecker.hpp"

namespace TestTypeChecker
{
    void TestAll();

    Ptr<Scope> InitializeBasicTypes();

    void OutputTypeRecord(std::string path, const HashMap<int, Ptr<Value>> &typeRecord);

    void Test1();
}
#endif //TESTTYPECHECKER_HPP
