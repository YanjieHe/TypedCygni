#include "Location.hpp"

namespace cygni
{
	Location::Location(LocationType type) :type{ type }
	{
	}
	ParameterLocation::ParameterLocation(int offset) : Location(LocationType::LocalVariable), offset{ offset }
	{
	}
	MemberLocation::MemberLocation(LocationType type, PackageRoute route, std::u32string typeName, std::u32string memberName,
		int index, int offset) : Location(type), route{ route }, typeName{ typeName }, memberName{ memberName }, index{ index }, offset{ offset }
	{
	}
	TypeLocation::TypeLocation(LocationType type, int index) : Location(type), index{ index }
	{
	}
}