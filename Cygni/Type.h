#ifndef TYPE_H
#define TYPE_H

#include "TypeTag.h"
#include <vector>
#include <string>

using std::vector;
using std::wstring;

class Type
{
  public:
	TypeTag tag;
	Type(TypeTag tag);
	virtual ~Type() { }
	virtual bool Matches(Type* other) = 0;
	virtual bool IsSubtypeOf(Type* other) = 0;
	virtual wstring ToString() = 0;
	virtual Type* Clone() = 0;

	static Type* Unknown();
	static Type* Int();
	static Type* Long();
	static Type* Float();
	static Type* Double();
	static Type* Boolean();
	static Type* Char();
	static Type* String();
	static Type* Unit();
};

class BasicType: public Type
{
  public:
	BasicType(TypeTag tag);
	~BasicType() override { }
	bool Matches(Type* other) override;
	bool IsSubtypeOf(Type* other) override;
	wstring ToString() override;
	Type* Clone() override;

	static Type* FromString(wstring name);
};

class ArrayType: public Type
{
  public:
	Type* element;
	ArrayType(Type* element);
	~ArrayType() override;
	bool Matches(Type* other) override;
	bool IsSubtypeOf(Type* other) override;
	wstring ToString() override;
	Type* Clone() override;
};

class FunctionType: public Type
{
  public:
	vector<Type*> parameters;
	Type* returnType;
	FunctionType(vector<Type*> parameters, Type* returnType);
	~FunctionType() override;
	bool Matches(Type* other) override;
	bool IsSubtypeOf(Type* other) override;
	wstring ToString() override;
	Type* Clone() override;

  private:
	bool ParametersMatch(FunctionType* other);
};

class BaseType: public Type
{
  public:
	wstring name;
	BaseType(wstring name);
	~BaseType() override { }
	bool Matches(Type* other) override;
	bool IsSubtypeOf(Type* other) override;
	wstring ToString() override;
	Type* Clone() override;
};

class InheritedType: public Type
{
  public:
	wstring name;
	Type* base;
	InheritedType(wstring name, Type* base);
	~InheritedType() override;
	bool Matches(Type* other) override;
	bool IsSubtypeOf(Type* other) override;
	wstring ToString() override;
	Type* Clone() override;
};

#endif // TYPE_H 
