#include "StringBuilder.h"

StringBuilder::StringBuilder()
{
}

void StringBuilder::Append(wchar_t c)
{
    vec.push_back(c);
}

void StringBuilder::Append(wstring& str)
{
    for (wchar_t item : str)
	{
        vec.push_back(item);
	}
}

void StringBuilder::Clear()
{
    vec.clear();
}

wstring StringBuilder::ToString()
{
	wstring result;
    result.reserve(vec.size());

    for (wchar_t item : vec)
	{
		result += item;
	}
	return result;
}
