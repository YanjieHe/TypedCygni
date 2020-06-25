#ifndef CYGNI_SOURCE_POSITION
#define CYGNI_SOURCE_POSITION
#include <memory>
#include <vector>
#include <string>

namespace cygni
{
	class FileLocation
	{
	public:
		std::string filePath;
		std::string fileName;
		FileLocation(std::string filePath, std::string fileName);
	};

	class SourcePosition
	{
	public:
		std::shared_ptr<FileLocation> document;
		int startLine;
		int startCol;
		int endLine;
		int endCol;

		SourcePosition();
		SourcePosition(std::shared_ptr<FileLocation> document, int startLine,
			int startCol, int endLine, int endCol);
	};

	using PackageRoute = std::vector<std::u32string>;

	std::u32string PackageRouteToString(const PackageRoute& route);

	//using FullQualifiedName = std::vector<std::u32string>;
	//std::u32string FullQualifiedNameToString(const FullQualifiedName& name);

	class FullQualifiedName
	{
	public:
		std::vector<std::u32string> name;

		FullQualifiedName();

		FullQualifiedName(PackageRoute route);

		std::u32string ToString();

		inline size_t Size() const
		{
			return name.size();
		}

		FullQualifiedName Concat(std::u32string s) const;
	};

} // namespace cygni

namespace std
{
	template <>
	struct hash<cygni::PackageRoute>
	{
		std::hash<std::u32string> hashFunction;
		std::size_t operator()(const cygni::PackageRoute& key) const
		{
			std::size_t seed = key.size();
			for (const auto& i : key)
			{
				seed = seed ^ (hashFunction(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			return seed;
		}
	};

	template <>
	struct equal_to<cygni::PackageRoute>
	{
		bool operator()(const cygni::PackageRoute& lhs, const cygni::PackageRoute& rhs) const
		{
			if (lhs.size() == rhs.size())
			{
				for (int i = 0; i < static_cast<int>(lhs.size()); i++)
				{
					if (lhs[i] != rhs[i])
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}
	};

	template <>
	struct hash<cygni::FullQualifiedName>
	{
		std::hash<std::u32string> hashFunction;
		std::size_t operator()(const cygni::FullQualifiedName& key) const
		{
			std::size_t seed = key.name.size();
			for (const auto& i : key.name)
			{
				seed = seed ^ (hashFunction(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			return seed;
		}
	};

	template <>
	struct equal_to<cygni::FullQualifiedName>
	{
		bool operator()(const cygni::FullQualifiedName& lhs, const cygni::FullQualifiedName& rhs) const
		{
			if (lhs.name.size() == rhs.name.size())
			{
				for (size_t i = 0; i < lhs.name.size(); i++)
				{
					if (lhs.name[i] != rhs.name[i])
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}
	};
};
#endif // CYGNI_SOURCE_POSITION