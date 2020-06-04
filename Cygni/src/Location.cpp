#include "Location.hpp"

namespace cygni
{
	Location::Location(LocationType type) :type{ type }
	{
	}
	ParameterLocation::ParameterLocation(int offset) : Location(LocationType::LocalVariable), offset{ offset }
	{
	}
	MemberLocation::MemberLocation(LocationType type, int index, int offset) : Location(type), index{ index }, offset{ offset }
	{
	}
	TypeLocation::TypeLocation(LocationType type, int index) : Location(type), index{ index }
	{
	}
}