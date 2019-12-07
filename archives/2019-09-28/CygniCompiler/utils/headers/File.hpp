#ifndef FILE_HPP
#define FILE_HPP

#include "Predef.hpp"
#include "String.hpp"

class File {
  typedef uint8_t Byte;

 public:
  static String ReadFile(const String& path);
  static void WriteBinaryFile(const String& path, const Vector<Byte>& bytes);
};

#endif  // FILE_HPP
