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
