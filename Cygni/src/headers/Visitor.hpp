#ifndef CYGNI_VISITOR_HPP
#define CYGNI_VISITOR_HPP
#include "Expression.hpp"
#include <nlohmann/json.hpp>

namespace cygni {
using json = nlohmann::json;

class AstToJsonSerialization {
public:
	json VisitSourceLocation(SourceLocation location);
	json VisitBinary(std::shared_ptr<BinaryExpression> node);
	json VisitBlock(std::shared_ptr<BlockExpression> node);
	json VisitExpression(ExpPtr node);
	json VisitConstant(std::shared_ptr<ConstantExpression> node);
	json VisitClassInfo(std::shared_ptr<ClassInfo> info);
	json VisitFieldDef(const FieldDef& field);
	json VisitMethodDef(const MethodDef& method);
	json VisitParameter(std::shared_ptr<ParameterExpression> parameter);
    json VisitReturn(std::shared_ptr<ReturnExpression> node);
	json VisitProgram(const Program& program);
};

} // namespace cygni

#endif // CYGNI_VISITOR_HPP