#include "DebugInfo.h"

Position::Position(int line, int column) : line{line}, column{column}
{
}

DebugInfo::DebugInfo()
{
	this->expList = vector<Expression*>();
    this->positions = vector<Position>();
}

DebugInfo& DebugInfo::operator=(DebugInfo&& other)
{
    expList = other.expList;
    positions = other.positions;
    other.expList.clear();
    other.positions.clear();
    return *this;
}

DebugInfo::~DebugInfo()
{
    for (Expression* item : expList)
    {
        delete item;
    }
}

DebugInfo::DebugInfo(DebugInfo&& other)
    : expList{other.expList}, positions{other.positions}
{
}

void DebugInfo::Record(int line, int column, Expression* expression)
{
    int id = static_cast<int>(expList.size());
	expression->ID = id;
	expList.push_back(expression);
    positions.push_back(Position(line, column));
}

void DebugInfo::Record(Position position, Expression* expression)
{
    int id = static_cast<int>(expList.size());
    expression->ID = id;
    expList.push_back(expression);
    positions.push_back(position);
}

Position DebugInfo::Locate(Expression* expression)
{
    return positions.at(static_cast<unsigned long>(expression->ID));
}
