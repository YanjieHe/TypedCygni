#ifndef FILE_HPP
#define FILE_HPP


#include "String.hpp"

class File
{
public:
    static String ReadFile(const char *path);
};


#endif //FILE_HPP
