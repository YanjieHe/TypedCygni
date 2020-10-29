#ifndef SCOPE_HPP
#define SCOPE_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

using std::unordered_map;
using std::shared_ptr;
using std::string;
using std::optional;
using std::make_optional;

template <typename T> class Scope {
public:
  typedef shared_ptr<Scope<T>> Ptr;

  unordered_map<string, T> table;
  Scope<T>::Ptr parent;

  Scope() : parent() {}

  explicit Scope(Scope<T>::Ptr parent) : parent(parent) {}

  optional<T> Get(string key) const {
    if (table.count(key)) {
      return table[key];
    } else {
      if (parent) {
        return parent->Get(key);
      } else {
        return make_optional<T>();
      }
    }
  }

  /* true: success, false: failed */
  bool Put(string key, const T &value) {
    if (table.count(key)) {
      return false;
    } else {
      table.insert({key, value});
      return true;
    }
  }
};

#endif // SCOPE_HPP