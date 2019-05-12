#include "File.hpp"
#include <fstream>

String File::ReadFile(const char *path)
{
    std::ifstream stream(path, std::ios::binary);

    auto utf8Characters = std::string{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
    return String(utf8Characters);
}
