#ifndef TABLE_HPP
#define TABLE_HPP

template <typename TKey, typename TValue> class Table {
private:
  std::unordered_map<String, int> map;
  std::vector<TValue> values;

public:
  Table() = default;
  void Add(const TKey &key, const TValue &value) {
    int n = values.size();
    map.insert({key, n});
    values.push_back(value);
  }
  inline bool HasKey(const TKey &key) { return map.find(key) != map.end(); }
  inline const TValue &GetByKey(const TKey &key) const {
    return values[map[key]];
  }
  inline TValue &GetByKey(const TKey &key) { return values[map[key]]; }
  inline const TValue &GetByIndex(int index) const { return values[index]; }
  inline TValue &GetByIndex(int index) { return values[index]; }
  inline int Size() { return values.size(); }
  inline int Size() const { return values.size(); }
};

#endif // TABLE_HPP