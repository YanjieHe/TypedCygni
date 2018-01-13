#include "Location.h"
#include "Exception.h"
#include <sstream>

using namespace std;

Location::Location(LocationKind kind, int index) : kind{kind}, index{index}
{
}

Location::Location(const Location& location)
    : kind{location.kind}, index{location.index}
{
}

Location::Location() : kind{LocationKind::Unknown}, index{-1}
{
}

wstring Location::ToString()
{
	wostringstream oss;
    if (kind == LocationKind::Unknown)
    {
        oss << L"[Unknown, ";
    }
    else if (kind == LocationKind::Global)
    {
        oss << L"[Global, ";
    }
    else if (kind == LocationKind::InFunction)
    {
        oss << L"[InFunction, ";
    }
    else if (kind == LocationKind::FunctionID)
    {
        oss << L"[FunctionID, ";
    }
    else
    {
        throw NotImplementedException();
    }
	oss << index << L"]";
    return oss.str();
}

LocationRecord::LocationRecord()
{
    this->record = map<int, Location>();
}

void LocationRecord::Record(ExpressionPtr& expression, Location location)
{
    record.insert(map<int, Location>::value_type(expression->ID, location));
}
void LocationRecord::Record(Expression* expression, Location location)
{
    record.insert(map<int, Location>::value_type(expression->ID, location));
}
Location LocationRecord::Find(ExpressionPtr& expression)
{
    auto iter = record.find(expression->ID);
    if (iter != record.end())
    {
        return record[expression->ID];
    }
    else
    {
        return Location();
    }
}

Location LocationRecord::Find(Expression* expression)
{
    auto iter = record.find(expression->ID);
    if (iter != record.end())
    {
        return record[expression->ID];
    }
    else
    {
        return Location();
    }
}
