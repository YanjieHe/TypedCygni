#ifndef PREDEF_HPP
#define PREDEF_HPP

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <iostream>
#include "Encoding.hpp"

template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

template<class T, class R>
std::shared_ptr<T> Cast(const std::shared_ptr<R> &other)
{
    return std::static_pointer_cast<T>(other);
}

template<class _Ty, class... _Types>
inline std::shared_ptr<_Ty> New(_Types &&... _Args)
{
    return std::make_shared<_Ty>(_Args...);
}

using i32 = int32_t;
using i64 = int64_t;

using f32 = float_t;
using f64 = double_t;

using u32 = uint32_t;
using u64 = uint64_t;

template<typename T>
using Vector = std::vector<T>;

template<typename TKey, typename TValue>
using HashMap = std::unordered_map<TKey, TValue>;

template<class _This, class... _Rest>
using Tuple = std::tuple<_This, _Rest...>;

template<class... _Types>
constexpr std::tuple<std::_Unrefwrap_t<_Types>...> tuple(_Types &&... _Args)
{
    return std::make_tuple(_Args...);
}

using std::tie;

class Enumerate
{
public:
    template<typename Selector, typename T>
    static auto Map(const Vector<T> &input, Selector selector)
    {
        Vector<decltype(selector(input.front()))> result;
        result.reserve(input.size());
        for (const auto &item: input)
        {
            result.push_back(selector(item));
        }
        return result;
    }
};

#endif //PREDEF_HPP
