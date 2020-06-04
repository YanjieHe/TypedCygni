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
		int index; // module or class
		int offset;
		MemberLocation(LocationType type, int index, int offset);
	};

	class TypeLocation : public Location
	{
	public:
		int index; // module or class
		TypeLocation(LocationType type, int index);
	};
}

#endif // CYGNI_LOCATION_HPP