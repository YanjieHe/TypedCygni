#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include "Expression.h"
#include "Position.h"
#include <vector>

using std::vector;

class DebugInfo
{
public:
	vector<Position> positions;
    DebugInfo();
    void Record(Position position, ExpressionPtr& expression);
    void Record(Position position, Expression* expression);
    Position Locate(ExpressionPtr& expression);
    Position Locate(Expression* expression);
};

#endif // DEBUGINFO_H
