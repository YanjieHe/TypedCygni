#ifndef CYGNI_TYPE_HPP
#define CYGNI_TYPE_HPP
#include "Enum.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
namespace cygni {
class Type {
public:
	TypeCode typeCode;

	explicit Type(TypeCode typeCode);
};

using TypePtr = std::shared_ptr<Type>;

class UnknownType : public Type {
public:
    UnknownType();
};

class Int32Type : public Type {
public:
	Int32Type();
};

class Int64Type : public Type {
public:
	Int64Type();
};

class DoubleType : public Type {
public:
	DoubleType();
};

class CharType : public Type {
public:
	CharType();
};

class BooleanType : public Type {
public:
	BooleanType();
};

class StringType : public Type {
public:
	StringType();
};

class ObjectType : public Type {
public:
	std::u32string name;
	explicit ObjectType(std::u32string name);
};

template <typename TKey, typename TValue> class Table {
public:
	std::vector<TValue> values;
	std::unordered_map<TKey, int> map;

	Table() = default;
	void Add(const TKey& key, const TValue& value) {
		if (map.find(key) != map.end()) {
			values[map[key]] = value;
		} else {
			int n = values.size();
			map.insert({key, n});
			values.push_back(value);
		}
	}

	const TValue& GetValueByKey(const TKey& key) const {
		return values[map[key]];
	}

	TValue& GetValueByKey(const TKey& key) {
		return values[map[key]];
	}

	int GetIndexByKey(const TKey& key) {
		return map[key];
	}

	bool ContainsKey(const TKey& key) {
		return map.find(key) != map.end();
	}
};

class ArrayType {
public:
	TypePtr elementType;
	explicit ArrayType(TypePtr elementType);
};

class TypeParameter {
public:
	std::u32string name;
};

class GenericType {
public:
	std::vector<TypeParameter> parameters;
};

} // namespace cygni
#endif // CYGNI_TYPE_HPP