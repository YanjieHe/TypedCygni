#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

#include <vector>
#include <string>

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

	~StringBuilder();

  private:
	vector<wchar_t>* vec;
};

#endif // STRINGBUILDER_H 
