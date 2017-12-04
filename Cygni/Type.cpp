#include "Type.h"

Type::Type(TypeTag tag)
	:tag{tag}
{
}

Type* Type::Unknown()
{
	return new BasicType(TypeTag::Unknown);
}

Type* Type::Int()
{
	return new BasicType(TypeTag::Int);
}

Type* Type::Long()
{
	return new BasicType(TypeTag::Long);
}

Type* Type::Float()
{
	return new BasicType(TypeTag::Float);
}

Type* Type::Double()
{
	return new BasicType(TypeTag::Double);
}

Type* Type::Boolean()
{
	return new BasicType(TypeTag::Boolean);
}

Type* Type::Char()
{
	return new BasicType(TypeTag::Char);
}

Type* Type::String()
{
	return new BasicType(TypeTag::String);
}

BasicType::BasicType(TypeTag tag)
	:Type(tag)
{
}

bool BasicType::Matches(Type* other)
{
	return this->tag == other->tag;
}

bool BasicType::IsSubtypeOf(Type* other)
{
	if (this->tag == TypeTag::Int && this->tag == TypeTag::Double)
	{
		return true;
	}
	else
	{
		return false;
	}
}

wstring BasicType::ToString()
{
	return type_tag_to_wstring(tag);
}

ArrayType::ArrayType(Type* element)
	:Type(TypeTag::Array)
{
	this->element = element;
}

ArrayType::~ArrayType()
{
	delete this->element;
}

bool ArrayType::Matches(Type* other)
{
	if (other->tag == TypeTag::Array)
	{
		return element->Matches(((ArrayType*)other)->element);
	}
	else
	{
		return false;
	}
}

bool ArrayType::IsSubtypeOf(Type*)
{
	return false;
}

wstring ArrayType::ToString()
{
	return L"Array[" + element->ToString() + L"]";
}

FunctionType::FunctionType(vector<Type*>* parameters, Type* returnType)
	:Type(TypeTag::Function)
{
	this->parameters = parameters;
	this->returnType = returnType;
}

FunctionType::~FunctionType()
{
	for (Type* item: *parameters)
	{
		delete item;
	}
	delete parameters;
	delete returnType;
}

bool FunctionType::Matches(Type* other)
{
	if (other->tag == TypeTag::Function)
	{
		FunctionType* ft = (FunctionType*) other;
		if (parameters->size() == ft->parameters->size())
		{
			return ParametersMatch(ft) && returnType->Matches(ft);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

wstring FunctionType::ToString()
{
	wstring result = L"Func[";
	if (parameters->size() == 0)
	{
		result += returnType->ToString();
		result += L"]";
		return result;
	}
	else
	{
		result += parameters->at(0)->ToString();
		for (unsigned int i = 1; i < parameters->size(); i++)
		{
			result += L", ";
			result += parameters->at(1)->ToString();
		}
		result += L", ";
		result += returnType->ToString();
		result += L"]";
		return result;
	}
}

bool FunctionType::IsSubtypeOf(Type*)
{
	return false;
}

bool FunctionType::ParametersMatch(FunctionType* other)
{
	unsigned int i = 0;
	bool match = true;
	while (match && i < parameters->size())
	{
		match = parameters->at(i)->Matches(other->parameters->at(i));
		i++;
	}
	return match;
}

BaseType::BaseType(wstring name)
	:Type(TypeTag::Base), name{name}
{
}

bool BaseType::Matches(Type* other)
{
	if (other->tag == TypeTag::Base)
	{
		return name == ((BaseType*) other)->name;
	}
	else
	{
		return false;
	}
}

bool BaseType::IsSubtypeOf(Type*)
{
	return false;
}

wstring BaseType::ToString()
{
	return name;
}

InheritedType::InheritedType(wstring name, Type* base)
	:Type(TypeTag::Inherited), name{name}, base{base}
{
}

InheritedType::~InheritedType()
{
	delete this->base;
}

bool InheritedType::Matches(Type* other)
{
	if (other->tag == TypeTag::Inherited)
	{
		return name == ((InheritedType*) other)->name;
	}
	else
	{
		return false;
	}
}

bool InheritedType::IsSubtypeOf(Type* other)
{
	if (base->Matches(other))
	{
		return true;
	}
	else if (base->IsSubtypeOf(other))
	{
		return true;
	}
	else
	{
		return false;
	}
}

wstring InheritedType::ToString()
{
	return name;
}
