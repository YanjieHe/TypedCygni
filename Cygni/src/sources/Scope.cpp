#include "Scope.hpp"

namespace cygni {
Scope::Scope(std::shared_ptr<Scope> parent) : parent{parent} {
}

std::optional<std::any> Scope::Get(std::u32string key) {
	if (table.ContainsKey(key)) {
		return table.GetValueByKey(key);
	} else {
		if (parent) {
			return parent->Get(key);
		} else {
			return std::optional<std::any>();
		}
	}
}

void Scope::Put(std::u32string key, std::any value) {
	table.Add(key, value);
}
}