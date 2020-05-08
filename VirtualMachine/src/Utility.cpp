#include "Utility.hpp"
#include <iostream>
#include <fstream>

std::vector<Byte> ReadBytesFromFile(std::string path)
{
	std::ifstream stream(path, std::ios_base::binary);

	return std::vector<Byte>(std::istreambuf_iterator<char>(stream),
		std::istreambuf_iterator<char>());
}