#ifndef TEST_TYPE_CHECKER_HPP
#define TEST_TYPE_CHECKER_HPP

#include "TypeChecker.hpp"

namespace TestTypeChecker {
void TestAll();


TypeChecker::ScopeCollection InitializeScopeCollection();

Ptr<Scope<Vector<Ptr<FunctionType>>>> InitializeOperatorScope();

Ptr<Scope<Ptr<Type>>> InitializeTypeScope();

void OutputTypeRecord(const std::string& path,
                      const HashMap<int, Ptr<Type>>& typeRecord);

void Test1();
}  // namespace TestTypeChecker
#endif  // TEST_TYPE_CHECKER_HPP
