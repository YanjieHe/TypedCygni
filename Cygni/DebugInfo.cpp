#include "DebugInfo.h"

DebugInfo::DebugInfo()
{
    this->positions = vector<Position>();
}

void DebugInfo::Record(Position position, ExpressionPtr& expression)
{
    int id = static_cast<int>(positions.size());
    expression->ID = id;
    positions.push_back(position);
}

void DebugInfo::Record(Position position, Expression* expression)
{
    int id = static_cast<int>(positions.size());
    expression->ID = id;
    positions.push_back(position);
}

Position DebugInfo::Locate(ExpressionPtr& expression)
{
    return positions.at(static_cast<unsigned int>(expression->ID));
}

Position DebugInfo::Locate(Expression* expression)
{
    return positions.at(static_cast<unsigned int>(expression->ID));
}
