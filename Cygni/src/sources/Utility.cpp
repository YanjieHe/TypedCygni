#include "Utility.hpp"
#include "Exception.hpp"

namespace cygni {

int HexToInt(std::u32string hex) {
	int val = 0;
	for (int i = 0; i < hex.size(); i++) {
		val = val * 16;
		if (hex[i] >= U'0' && hex[i] <= U'9') {
			val = val + (hex[i] - U'0');
		} else if (hex[i] >= 'a' && hex[i] <= U'f') {
			val = val + (hex[i] - U'a');
		} else if (hex[i] >= 'A' && hex[i] <= U'F') {
			val = val + (hex[i] - U'F');
		} else {
			throw ArgumentException(U"hex digit character out of range");
		}
	}
	return val;
}

} // namespace cygni
