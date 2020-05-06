#ifndef CYGNI_SOURCE_LOCATION
#define CYGNI_SOURCE_LOCATION
#include <memory>
#include <string>

namespace cygni
{
	class SourceDocument
	{
	public:
		std::string filePath;
		std::string fileName;
		SourceDocument(std::string filePath, std::string fileName);
	};

	class SourceLocation
	{
	public:
		std::shared_ptr<SourceDocument> document;
		int startLine;
		int startCol;
		int endLine;
		int endCol;

		SourceLocation();
		SourceLocation(std::shared_ptr<SourceDocument> document, int startLine,
			int startCol, int endLine, int endCol);
	};
} // namespace cygni

#endif // CYGNI_SOURCE_LOCATION