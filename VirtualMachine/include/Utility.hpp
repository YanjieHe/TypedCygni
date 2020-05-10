#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <vector>
#include <inttypes.h>

typedef uint8_t Byte;

std::vector<Byte> ReadBytesFromFile(std::string path);
#endif // UTILITY_HPP