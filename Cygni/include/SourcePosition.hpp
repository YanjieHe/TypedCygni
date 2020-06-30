#ifndef CYGNI_SOURCE_POSITION
#define CYGNI_SOURCE_POSITION
#include <memory>
#include <string>
#include <vector>

namespace cygni {
class FileLocation {
public:
  std::string filePath;
  std::string fileName;
  FileLocation(std::string filePath, std::string fileName);
};

class SourcePosition {
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

class Route {
protected:
  virtual const std::vector<std::u32string> &GetRoute() const = 0;
  virtual std::vector<std::u32string> &GetRoute() = 0;

public:
  std::u32string ToString() const;
  size_t GetHashCode() const;
  void Add(const std::u32string &text);
  const std::u32string &Back() const;
  void PopBack();
  virtual ~Route() {}
};

class PackageRoute : public Route {
public:
  std::vector<std::u32string> route;
  PackageRoute();

  PackageRoute(std::vector<std::u32string> route);
  ~PackageRoute(){}

protected:
  const std::vector<std::u32string> &GetRoute() const override;
  std::vector<std::u32string> &GetRoute() override;
};

class FullQualifiedName : public Route {
public:
  std::vector<std::u32string> name;

  FullQualifiedName();

  FullQualifiedName(PackageRoute route);

  FullQualifiedName Concat(std::u32string s) const;

  ~FullQualifiedName(){}

protected:
  const std::vector<std::u32string> &GetRoute() const override;
  std::vector<std::u32string> &GetRoute() override;
};

bool operator==(const PackageRoute &lhs, const PackageRoute &rhs);
bool operator==(const FullQualifiedName &lhs, const FullQualifiedName &rhs);
} // namespace cygni

namespace std {
template <> struct hash<cygni::PackageRoute> {
  std::size_t operator()(const cygni::PackageRoute &key) const {
    return key.GetHashCode();
  }
};

template <> struct hash<cygni::FullQualifiedName> {
  std::size_t operator()(const cygni::FullQualifiedName &key) const {
    return key.GetHashCode();
  }
};
};     // namespace std
#endif // CYGNI_SOURCE_POSITION