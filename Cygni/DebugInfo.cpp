#include "DebugInfo.h"

Position::Position(int line, int column)
	:line{line}, column{column}
{
}

DebugInfo::DebugInfo()
{
	this->expList = vector<Expression*>();
	this->positions = vector<Position>();
}

void DebugInfo::Record(int line, int column, Expression* expression)
{
	int id = expList.size();
	expression->ID = id;
	expList.push_back(expression);
	positions.push_back(Position(line, column));
}
