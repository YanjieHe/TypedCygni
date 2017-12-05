#include "Location.h"
#include <sstream>

using namespace std;

Location::Location(LocationKind kind, int index)
	:kind{kind}, index{index}
{
}

Location::Location(const Location& location)
	:kind{location.kind}, index{location.index}
{
}

Location::Location()
	:kind{LocationKind::Unknown}, index{-1}
{
}

wstring Location::ToString()
{
	wostringstream oss;
	switch (kind)
	{
		case LocationKind::Unknown:
			oss << L"[Unknown, ";
			break;
		case LocationKind::Global:
			oss << L"[Global, ";
			break;
		case LocationKind::Function:
			oss << L"[Function, ";
			break;
		default:
		case LocationKind::Class:
			oss << L"[Class, ";
			break;
	}
	oss << index << L"]";
	return oss.str();
}
