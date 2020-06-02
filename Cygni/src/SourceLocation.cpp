#include "SourceLocation.hpp"

namespace cygni
{
	FileLocation::FileLocation(std::string filePath, std::string fileName)
		: filePath{ filePath }, fileName{ fileName } {
	}

	SourceLocation::SourceLocation()
		: document(), startLine{ 0 }, startCol{ 0 }, endLine{ 0 }, endCol{ 0 } {
	}

	SourceLocation::SourceLocation::SourceLocation(
		std::shared_ptr<FileLocation> document, int startLine, int startCol,
		int endLine, int endCol)
		: document{ document }, startLine{ startLine }, startCol{ startCol },
		endLine{ endLine }, endCol{ endCol } {
	}

	std::u32string PackageRouteToString(const PackageRoute & route)
	{
		if (route.empty())
		{
			return U"";
		}
		else
		{
			std::u32string text;
			size_t size = 0;
			for (size_t i = 0; i < route.size(); i++)
			{
				size = size + route[i].size();
			}
			size = size + route.size() - 1;
			text.reserve(size);

			for (auto c : route.front())
			{
				text.push_back(c);
			}
			for (size_t i = 1; i < route.size(); i++)
			{
				text.push_back(U'.');
				for (auto c : route.at(i))
				{
					text.push_back(c);
				}
			}
			return text;
		}
	}

} // namespace cygni