#ifndef CYGNI_UTILITY_HPP
#define CYGNI_UTILITY_HPP
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <cstdint>

namespace cygni
{
	inline static bool IsDigit(char32_t c) { return (c >= U'0' && c <= U'9'); }

	inline static bool IsLetter(char32_t c)
	{
		return (c >= U'a' && c <= U'z') || (c >= U'A' && c <= U'Z');
	}

	inline static bool IsWhiteSpace(char32_t c)
	{
		return c == U' ' || c == U'\t' || c == U'\v' || c == U'\r' || c == U'\n';
	}

	int HexToInt(std::u32string hex);

	std::string UTF32ToUTF8(const std::u32string &utf32);

	std::u32string UTF8ToUTF32(const std::string &utf8);

	template <typename TKey, typename TValue>
	class Table
	{
	public:
		std::vector<TValue> values;
		std::unordered_map<TKey, int> map;

		Table() = default;

		void Add(const TKey &key, const TValue& value)
		{
			if (map.find(key) != map.end())
			{
				values[map[key]] = value;
			}
			else
			{
				int n = static_cast<int>(values.size());
				map.insert({ key, n });
				values.push_back(value);
			}
		}

		const TValue &GetValueByKey(const TKey &key) const
		{
			return values[map.at(key)];
		}

		TValue &GetValueByKey(const TKey &key) { return values[map[key]]; }

		int GetIndexByKey(const TKey &key)
		{
			if (map.find(key) != map.end())
			{
				return map[key];
			}
			else
			{
				return -1;
			}
		}

		bool ContainsKey(const TKey &key) const { return map.find(key) != map.end(); }

		int Size() const
		{
			return static_cast<int>(values.size());
		}

		typename std::vector<TValue>::iterator begin()
		{
			return values.begin();
		}

		typename std::vector<TValue>::iterator end()
		{
			return values.end();
		}

		typename std::vector<TValue>::const_iterator begin() const
		{
			return values.begin();
		}

		typename std::vector<TValue>::const_iterator end() const
		{
			return values.end();
		}

		void Clear()
		{
			values.clear();
			map.clear();
		}
	};

	std::string ReadText(std::string path);
	void WriteText(std::string path, std::string text);
	void WriteBytes(std::string path, const std::vector<uint8_t>& bytes);


	class Convert
	{
	public:
		static std::u32string ToString(const int32_t& i);
		static std::u32string ToString(const char32_t& c);
		static std::u32string ToString(const std::u32string& s);
		static std::u32string ToString(const std::string& s);
	};

	std::u32string FormatInternal(std::u32string fmt, std::basic_ostringstream<char32_t>& stream, int i);

	template<class T, class... Args>
	std::u32string FormatInternal(std::u32string fmt, std::basic_ostringstream<char32_t>& stream, int i, const T& head, const Args&... rest)
	{
		int n = static_cast<int>(fmt.size());
		while (i < n)
		{
			if (fmt[i] == U'{')
			{
				if (i + 1 >= n)
				{
					throw std::invalid_argument("wrong format");
				}
				else
				{
					if (fmt[i + 1] == U'{')
					{
						// escape left brace
						stream.put(U'{');
						i = i + 2;
					}
					else if (fmt[i + 1] == U'}')
					{
						stream << Convert::ToString(head);
						i = i + 2;
						return FormatInternal(fmt, stream, i, rest...);
					}
					else
					{
						throw std::invalid_argument("expecting right brace for closure");
					}
				}
			}
			else if (fmt[i] == U'}')
			{
				if (i + 1 >= n)
				{
					throw std::invalid_argument("wrong format");
				}
				else
				{
					if (fmt[i + 1] == U'}')
					{
						stream.put(U'}');
						i = i + 2;
					}
					else
					{
						throw std::invalid_argument("expecting a right brace for escaping");
					}
				}
			}
			else
			{
				stream.put(fmt[i]);
				i = i + 1;
			}
		}
		return stream.str();
	}

	template <class... Args>
	std::u32string Format(std::u32string fmt, const Args&... args)
	{
		std::basic_ostringstream<char32_t> stream;
		int i = 0;
		return FormatInternal(fmt, stream, i, args...);
	}

} // namespace cygni

std::ostream &operator<<(std::ostream &stream, const std::u32string &utf32);

#endif // CYGNI_UTILITY_HPP
