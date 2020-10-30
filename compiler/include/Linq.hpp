#ifndef LINQ_HPP
#define LINQ_HPP
#include <iterator>
#include <vector>

// template <typename T>
// class IEnumerator : public std::enable_shared_from_this<IEnumerator<T>> {
// public:
//   typedef std::shared_ptr<IEnumerator> Ptr;

//   virtual bool MoveNext() = 0;
//   virtual T Current() = 0;

//   template <typename TSelector>
//   auto Select(TSelector selector) ->
//       typename IEnumerator<decltype(selector(T()))>::Ptr;
// };

// template <typename T, typename TIterator>
// class CollectionEnumerator : public IEnumerator<T> {
// public:
//   typedef std::shared_ptr<CollectionEnumerator> Ptr;

//   bool moved;
//   TIterator current;
//   TIterator last;

//   CollectionEnumerator(TIterator first, TIterator last)
//       : moved{false}, current{first}, last{last} {}

//   bool MoveNext() override {
//     if (current == last) {
//       return false;
//     } else {
//       if (moved) {
//         current++;
//         return current != last;
//       } else {
//         moved = true;
//         return true;
//       }
//     }
//   }

//   T Current() override { return *current; }
// };

// template <typename T, typename TSelector>
// class SelectEnumerator : public IEnumerator<T> {
// public:
//   typedef std::shared_ptr<SelectEnumerator> Ptr;

//   typename IEnumerator<T>::Ptr enumerator;
//   TSelector selector;
//   T result;

//   SelectEnumerator(typename IEnumerator<T>::Ptr enumerator, TSelector
//   selector)
//       : enumerator{enumerator}, selector{selector} {}

//   bool MoveNext() override {
//     if (enumerator->MoveNext()) {
//       result = selector(enumerator->Current());
//       return true;
//     } else {
//       return false;
//     }
//   }

//   T Current() override { return result; }
// };

// template <typename T>
// template <typename TSelector>
// auto IEnumerator<T>::Select(TSelector selector) ->
//     typename IEnumerator<decltype(selector(T()))>::Ptr {
//   return std::make_shared<SelectEnumerator<T, TSelector>>(
//       std::enable_shared_from_this<IEnumerator<T>>::shared_from_this(),
//       selector);
// }

namespace linq {
template <typename TIterator> class linq_enumerable;
template <typename TIterator, typename TSelector> class select_iterator;
template <typename TIterator, typename TPredicate> class where_iterator;
template <typename TIterator> class take_iterator;

template <typename TIterator>
using iterator_type = decltype(**((TIterator *) nullptr));

template <typename TIterator> class linq_enumerable {
public:
private:
  TIterator first;
  TIterator last;

public:
  linq_enumerable(TIterator first, TIterator last) : first{first}, last{last} {}

  TIterator begin() const { return first; }

  TIterator end() const { return last; }

  template <typename TSelector>
  auto select(const TSelector &selector) const
      -> linq_enumerable<select_iterator<TIterator, TSelector>>;

  template <typename TPredicate>
  auto where(const TPredicate &predicate) const
      -> linq_enumerable<where_iterator<TIterator, TPredicate>>;

  auto take(int count) const -> linq_enumerable<take_iterator<TIterator>>;

  std::vector<iterator_type<TIterator>> to_vector() const {
    std::vector<iterator_type<TIterator>> items;
    for (auto item : *this) {
      items.push_back(item);
    }
    return items;
  }
};

template <typename TContainer>
auto from(const TContainer &container)
    -> linq_enumerable<decltype(std::begin(container))> {
  return linq_enumerable<decltype(std::begin(container))>(std::begin(container),
                                                          std::end(container));
}

template <typename TIterator, typename TSelector> class select_iterator {
private:
  typedef select_iterator<TIterator, TSelector> TSelf;

  TIterator iterator;
  TSelector selector;

public:
  select_iterator(const TIterator &iterator, const TSelector &selector)
      : iterator{iterator}, selector{selector} {}

  TSelf &operator++() {
    ++iterator;
    return *this;
  }

  auto operator*() const -> decltype(selector(*iterator)) {
    return selector(*iterator);
  }

  bool operator==(const TSelf &it) const { return it.iterator == iterator; }

  bool operator!=(const TSelf &it) const { return it.iterator != iterator; }
};

template <typename TIterator, typename TPredicate> class where_iterator {
private:
  typedef where_iterator<TIterator, TPredicate> TSelf;

  TIterator iterator;
  TIterator last;
  TPredicate predicate;

public:
  where_iterator(const TIterator &it, const TIterator &last,
                 const TPredicate &predicate)
      : iterator{it}, last{last}, predicate{predicate} {
    while (iterator != last && !predicate(*iterator)) {
      ++iterator;
    }
  }

  TSelf &operator++() {
    if (iterator == last) {
      return *this;
    } else {
      ++iterator;
      while (iterator != last && !predicate(*iterator)) {
        ++iterator;
      }
      return *this;
    }
  }

  iterator_type<TIterator> operator*() const { return *iterator; }

  bool operator==(const TSelf &it) const { return it.iterator == iterator; }

  bool operator!=(const TSelf &it) const { return it.iterator != iterator; }
};

template <typename TIterator> class take_iterator {
private:
  typedef take_iterator<TIterator> TSelf;

  TIterator iterator;
  TIterator last;
  int count;
  int current;

public:
  take_iterator(const TIterator &_iterator, const TIterator &_last, int count,
                int current)
      : iterator{_iterator}, last{_last}, count{count}, current{current} {
  }

  iterator_type<TIterator> operator*() const { return *iterator; }

  TSelf &operator++() {
    current++;
    if (current == count) {
      return *this;
    } else {
      ++iterator;
      return *this;
    }
  }

  bool operator==(const TSelf &it) const { return it.current == current; }

  bool operator!=(const TSelf &it) const { return it.current != current; }
};

template <typename TIterator>
template <typename TSelector>
auto linq_enumerable<TIterator>::select(const TSelector &selector) const
    -> linq_enumerable<select_iterator<TIterator, TSelector>> {
  return linq_enumerable<select_iterator<TIterator, TSelector>>(
      select_iterator<TIterator, TSelector>(first, selector),
      select_iterator<TIterator, TSelector>(last, selector));
}

template <typename TIterator>
template <typename TPredicate>
auto linq_enumerable<TIterator>::where(const TPredicate &predicate) const
    -> linq_enumerable<where_iterator<TIterator, TPredicate>> {
  return linq_enumerable<where_iterator<TIterator, TPredicate>>(
      where_iterator<TIterator, TPredicate>(first, last, predicate),
      where_iterator<TIterator, TPredicate>(last, last, predicate));
}

template <typename TIterator>
auto linq_enumerable<TIterator>::take(int count) const
    -> linq_enumerable<take_iterator<TIterator>> {
  return linq_enumerable<take_iterator<TIterator>>(
      take_iterator<TIterator>(first, last, count, 0),
      take_iterator<TIterator>(last, last, count, count));
}

} // namespace linq
#endif // LINQ_HPP