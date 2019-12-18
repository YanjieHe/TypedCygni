#include "SourceLocation.hpp"

namespace cygni {
SourceDocument::SourceDocument(std::string filePath, std::string fileName)
	: filePath{filePath}, fileName{fileName} {
}

SourceLocation::SourceLocation()
	: document(), startLine{0}, startCol{0}, endLine{0}, endCol{0} {
}

SourceLocation::SourceLocation::SourceLocation(
	std::shared_ptr<SourceDocument> document, int startLine, int startCol,
	int endLine, int endCol)
	: document{document}, startLine{startLine}, startCol{startCol},
	  endLine{endLine}, endCol{endCol} {
}

} // namespace cygni