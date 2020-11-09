#ifndef ENCODING_HPP
#define ENCODING_HPP
#include <string>
#include <variant>
#include <stdexcept>

namespace encoding {

using std::string;
using std::u32string;
using std::variant;
using std::invalid_argument;

variant<u32string, invalid_argument> utf8_to_ut32(const string &utf8);
string utf32_to_utf8(const u32string &utf32);

} // namespace encoding

#endif // ENCODING_HPP