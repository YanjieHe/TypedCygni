#include "Location.hpp"

namespace cygni
{
	Location::Location(LocationType type) :type{ type }
	{
	}
	ParameterLocation::ParameterLocation(int offset) : Location(LocationType::LocalVariable), offset{ offset }
	{
	}
	MemberLocation::MemberLocation(LocationType type, FullQualifiedName name, int offset)
		: Location(type), name{ name }, offset{ offset }
	{
	}
	TypeLocation::TypeLocation(LocationType type, FullQualifiedName name) : Location(type), name{ name }
	{
	}
}