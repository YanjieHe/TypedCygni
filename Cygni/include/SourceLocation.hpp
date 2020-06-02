#ifndef CYGNI_SOURCE_LOCATION
#define CYGNI_SOURCE_LOCATION
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

	class SourceLocation
	{
	public:
		std::shared_ptr<FileLocation> document;
		int startLine;
		int startCol;
		int endLine;
		int endCol;

		SourceLocation();
		SourceLocation(std::shared_ptr<FileLocation> document, int startLine,
			int startCol, int endLine, int endCol);
	};

	using PackageRoute = std::vector<std::u32string>;

	std::u32string PackageRouteToString(const PackageRoute& route);
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
};
#endif // CYGNI_SOURCE_LOCATION