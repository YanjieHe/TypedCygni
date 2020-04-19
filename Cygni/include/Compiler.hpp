#ifndef CYGNI_COMPILER_HPP
#define CYGNI_COMPILER_HPP
#include <cstdint>
#include <vector>
#include "Expression.hpp"
namespace cygni {
	using Byte = uint8_t;

	enum class OpCode {

	};

	enum class TypeTag {
		TYPE_I32,
		TYPE_I64,
		TYPE_F32,
		TYPE_F64
	};

	class Compiler {
	public:
		void CompileExpression(ExpPtr node, std::vector<Byte>& byteCode);
	};
} // namespace cygni

#endif // CYGNI_COMPILER_HPP