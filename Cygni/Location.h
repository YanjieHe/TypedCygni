#ifndef LOCATION_H
#define LOCATION_H

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

#endif // LOCATION_H
