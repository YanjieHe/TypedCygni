#include "File.hpp"
#include <fstream>
#include "Exception.hpp"

String File::ReadFile(const String& path) {
  std::ifstream stream(path.ToCppString(), std::ios::binary);
  if (stream) {
    auto utf8Characters = std::string{std::istreambuf_iterator<char>(stream),
                                      std::istreambuf_iterator<char>()};
    stream.close();
    return String(utf8Characters);
  } else {
    throw FileNotFoundException(path);
  }
}

void File::WriteBinaryFile(const String& path, const Vector<Byte>& bytes) {
  std::ofstream stream(path.ToCppString(), std::ios::binary);
  if (stream) {
    for (auto byte : bytes) {
      stream.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }
    stream.close();
  } else {
    throw FileNotFoundException(path);
  }
}