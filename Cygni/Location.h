#ifndef LOCATION_H
#define LOCATION_H

#include "Expression.h"
#include <string>

using std::wstring;

enum class LocationKind
{
    Unknown,
    Global,
    InFunction,
    FunctionID
};

class Location
{
public:
	LocationKind kind;
	int index;
	Location(LocationKind kind, int index);
	Location(const Location& location);
	Location();
	wstring ToString();
};

class LocationRecord
{
public:
    LocationRecord();
    void Record(ExpressionPtr& expression, Location location);
    void Record(Expression* expression, Location location);
    Location Find(ExpressionPtr& expression);
    Location Find(Expression* expression);

private:
    map<int, Location> record;
};

#endif // LOCATION_H
