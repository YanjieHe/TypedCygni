#ifndef TYPETAG_H
#define TYPETAG_H

#include <string>

using std::wstring;

enum class TypeTag
{
	Unknown,
	Int,
	Long,
	Float,
	Double,
	Boolean,
	Char,
	String,
	Unit,
	Null,
	Array,
	Base,
	Inherited,
	Function,
	Any
};

wstring TypeTagToString(TypeTag tag);

#endif // TYPETAG_H
