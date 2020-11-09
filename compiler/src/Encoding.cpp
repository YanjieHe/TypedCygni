#include "Encoding.hpp"

namespace encoding {

variant<u32string, invalid_argument> utf8_to_ut32(const string &utf8) {
  u32string res;
  size_t i = 0;
  size_t n = utf8.size();
  while (i < n) {
    if ((utf8[i] & 0x80) == 0) {
      res.push_back(utf8[i]);
      i = i + 1;
    } else if ((utf8[i] & 0xE0) == 0xC0) {
      if (i + 1 < n) {
        res.push_back(((utf8[i] & 0x1F) << 6) | (utf8[i + 1] & 0x3F));
        i = i + 2;
      } else {
        return invalid_argument("The sequence is truncated.");
      }
    } else if ((utf8[i] & 0xF0) == 0xE0) {
      if (i + 2 < n) {
        res.push_back(((utf8[i] & 0x0F) << 12) | ((utf8[i + 1] & 0x3F) << 6) |
                      (utf8[i + 2] & 0x3F));
        i = i + 3;
      } else {
        return invalid_argument("The sequence is truncated.");
      }
    } else if ((utf8[i] & 0xF8) == 0xF0) {
      if (i + 3 < n) {
        res.push_back(((utf8[i] & 0x07) << 18) | ((utf8[i + 1] & 0x3F) << 12) |
                      ((utf8[i + 2] & 0x3F) << 6) | (utf8[i + 3] & 0x3F));
        i = i + 4;
      } else {
        return invalid_argument("The sequence is truncated.");
      }
    } else {
      return invalid_argument("Illegal starting byte");
    }
  }
  return res;
}

string utf32_to_utf8(const u32string &utf32) {
  string res;
  for (char32_t cp : utf32) {
    if (cp < 0x80) {
      res.push_back(cp & 0x7F);
    } else if (cp < 0x800) {
      res.push_back(((cp >> 6) & 0x1F) | 0xC0);
      res.push_back((cp & 0x3F) | 0x80);
    } else if (cp < 0x10000) {
      res.push_back(((cp >> 12) & 0x0F) | 0xE0);
      res.push_back(((cp >> 6) & 0x3F) | 0x80);
      res.push_back((cp & 0x3F) | 0x80);
    } else {
      res.push_back(((cp >> 18) & 0x07) | 0xF0);
      res.push_back(((cp >> 12) & 0x3F) | 0x80);
      res.push_back(((cp >> 6) & 0x3F) | 0x80);
      res.push_back((cp & 0x3F) | 0x80);
    }
  }
  return res;
}

} // namespace encoding