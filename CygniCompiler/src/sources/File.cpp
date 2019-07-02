#include "File.hpp"
#include <fstream>
#include "Exception.hpp"

String File::ReadFile(String path)
{
    std::ifstream stream(path.ToCppString(), std::ios::binary);
    if (stream)
    {
        auto utf8Characters = std::string{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
        return String(utf8Characters);
    }
    else
    {
        throw FileNotFoundException(path);
    }
}
