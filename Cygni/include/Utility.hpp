#ifndef CYGNI_UTILITY_HPP
#define CYGNI_UTILITY_HPP
#include <string>
#include <unordered_map>
#include <vector>

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
		void Add(const TKey &key, const TValue &value)
		{
			if (map.find(key) != map.end())
			{
				values[map[key]] = value;
			}
			else
			{
				int n = values.size();
				map.insert({ key, n });
				values.push_back(value);
			}
		}

		const TValue &GetValueByKey(const TKey &key) const
		{
			return values[map.at(key)];
		}

		TValue &GetValueByKey(const TKey &key) { return values[map[key]]; }

		int GetIndexByKey(const TKey &key) { return map[key]; }

		bool ContainsKey(const TKey &key) const { return map.find(key) != map.end(); }
	};

	std::string ReadText(std::string path);
	void WriteText(std::string path, std::string text);

} // namespace cygni

std::ostream &operator<<(std::ostream &stream, const std::u32string &utf32);


#endif // CYGNI_UTILITY_HPP
