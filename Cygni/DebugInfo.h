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

	void Record(int line, int column, Expression* expression);
	Position Locate(Expression* expression);
};


#endif // DEBUGINFO_H 
