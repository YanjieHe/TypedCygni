#ifndef CYGNI_SCOPE_HPP
#define CYGNI_SCOPE_HPP
#include "Utility.hpp"
#include <any>
#include <memory>
#include <optional>

namespace cygni {
class Scope {
public:
	std::shared_ptr<Scope> parent;
	Table<std::u32string, std::any> table;

	Scope() = default;
    
	explicit Scope(std::shared_ptr<Scope> parent);

	std::optional<std::any> Get(std::u32string key);

	void Put(std::u32string key, std::any value);

	std::optional<std::any> GetInLocal(std::u32string key);
};

using ScopePtr = std::shared_ptr<Scope>;

} // namespace cygni

#endif // CYGNI_SCOPE_HPP
