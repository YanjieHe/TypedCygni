#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <string>
#include <vector>

using std::vector;
using std::wstring;

class StringBuilder
{
  public:
    StringBuilder();

	void Append(wchar_t c);
	void Append(wstring& str);
	void Clear();
    wstring ToString();

  private:
    vector<wchar_t> vec;
};

#endif // STRINGBUILDER_H
