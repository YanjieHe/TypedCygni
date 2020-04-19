#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <cstdint>
#include <vector>
#include <string>
#include "OpCode.hpp"

using Byte = uint8_t;

typedef struct {
	union {
		int32_t i32_v;
		int64_t i64_v;
		float_t f32_v;
		double_t f64_v;
		void* obj;
	}u;
	uint8_t is_pointer;
} Value;

class Function {
public:
	std::string name;
	int numArgs;
	int locals;
	std::vector<Byte> code;
	std::vector<Value> constantPool;
};

class Object {
public:
	std::vector<Value> fields;
	std::vector<Function>* methods;
	Object* next;
};

class ClassInfo {
public:
	std::string name;
	int numOfFields;
	std::vector<Function> methods;
	std::vector<Value> constantPool;
};

class ModuleInfo {
public:
	std::string name;
	std::vector<Value> variables;
	std::vector<Function> functions;
	std::vector<Value> constantPool;
};

class Program {
public:
	std::vector<ClassInfo> classes;
	std::vector<ModuleInfo> modules;
	Function* entry;
};

#endif // STRUCTURES_HPP