#ifndef ENCODING_HPP
#define ENCODING_HPP
#include <string>
#include <variant>
#include <stdexcept>

using std::string;
using std::u32string;
using std::variant;
using std::invalid_argument;


variant<u32string, invalid_argument> UTF8ToUTF32(const string &utf8);
string UTF32ToUTF8(const u32string &utf32) ;
#endif // ENCODING_HPP