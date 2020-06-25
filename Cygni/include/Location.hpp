#ifndef CYGNI_LOCATION_HPP
#define CYGNI_LOCATION_HPP
#include "Enum.hpp"
#include <SourcePosition.hpp>
#include <memory>
namespace cygni
{
	class Location;
	using LocationPtr = std::shared_ptr<Location>;

	class Location
	{
	public:
		LocationType type;
		explicit Location(LocationType type);
	};

	class ParameterLocation : public Location
	{
	public:
		int offset;
		explicit ParameterLocation(int offset);
	};

	class MemberLocation : public Location
	{
	public:
		FullQualifiedName name;
		int offset;
		MemberLocation(LocationType type, FullQualifiedName name, int offset);
	};

	class TypeLocation : public Location
	{
	public:
		FullQualifiedName name;
		TypeLocation(LocationType type, FullQualifiedName name);
	};
}

#endif // CYGNI_LOCATION_HPP