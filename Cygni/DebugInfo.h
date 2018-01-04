#ifndef DEBUGINFO_H
#define DEBUGINFO_H

#include "Expression.h"
#include <vector>

using std::vector;

class Position
{
public:
	int line;
	int column;
	Position(int line, int column);
};

class DebugInfo
{
public:
	vector<Expression*> expList;
	vector<Position> positions;
    DebugInfo();
    DebugInfo(const DebugInfo&) = delete;
    DebugInfo& operator=(const DebugInfo&) = delete;
    DebugInfo(DebugInfo&& other);
    DebugInfo& operator=(DebugInfo&& other);
    ~DebugInfo();
	void Record(int line, int column, Expression* expression);
    void Record(Position position, Expression* expression);
    Position Locate(Expression* expression);
};

#endif // DEBUGINFO_H
