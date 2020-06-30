#include "SourcePosition.hpp"

namespace cygni {
FileLocation::FileLocation(std::string filePath, std::string fileName)
    : filePath{filePath}, fileName{fileName} {}

SourcePosition::SourcePosition()
    : document(), startLine{0}, startCol{0}, endLine{0}, endCol{0} {}

SourcePosition::SourcePosition::SourcePosition(
    std::shared_ptr<FileLocation> document, int startLine, int startCol,
    int endLine, int endCol)
    : document{document}, startLine{startLine}, startCol{startCol},
      endLine{endLine}, endCol{endCol} {}

size_t Route::GetHashCode() const {
  static std::hash<std::u32string> u32Hash;
  std::size_t seed = GetRoute().size();
  for (const auto &i : GetRoute()) {
    seed = seed ^ (u32Hash(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
  }
  return seed;
}

std::u32string Route::ToString() const {
  auto &route = GetRoute();
  if (route.empty()) {
    return U"";
  } else {
    std::u32string text;
    size_t size = 0;
    for (size_t i = 0; i < route.size(); i++) {
      size = size + route[i].size();
    }
    size = size + route.size() - 1;
    text.reserve(size);

    for (auto c : route.front()) {
      text.push_back(c);
    }
    for (size_t i = 1; i < route.size(); i++) {
      text.push_back(U'.');
      for (auto c : route.at(i)) {
        text.push_back(c);
      }
    }
    return text;
  }
}

void Route::Add(const std::u32string &text) {
  auto &route = GetRoute();
  route.push_back(text);
}

const std::u32string &Route::Back() const { return GetRoute().back(); }

void Route::PopBack() {
  auto &route = GetRoute();
  route.pop_back();
}
PackageRoute::PackageRoute() {}
PackageRoute::PackageRoute(std::vector<std::u32string> route) : route{route} {}

const std::vector<std::u32string> &PackageRoute::GetRoute() const {
  return route;
}

std::vector<std::u32string> &PackageRoute::GetRoute() { return route; }

FullQualifiedName::FullQualifiedName() : name() {}

FullQualifiedName::FullQualifiedName(PackageRoute route) : name{route.route} {}

const std::vector<std::u32string> &FullQualifiedName::GetRoute() const {
  return name;
}

std::vector<std::u32string> &FullQualifiedName::GetRoute() { return name; }
FullQualifiedName FullQualifiedName::Concat(std::u32string s) const {
  FullQualifiedName newName = *this;
  newName.name.push_back(s);
  return newName;
}

bool operator==(const PackageRoute& lhs, const PackageRoute& rhs) {
  return lhs.route == rhs.route;
}
bool operator==(const FullQualifiedName& lhs, const FullQualifiedName& rhs) {
  return lhs.name == rhs.name;
}
} // namespace cygni