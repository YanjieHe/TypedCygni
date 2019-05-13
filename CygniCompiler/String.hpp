#ifndef STRING_HPP
#define STRING_HPP

#include <cstdint>
#include <string>
#include "Encoding.hpp"
#include <iostream>

typedef uint16_t Char;
typedef uint8_t Utf8Char;
typedef uint16_t Utf16Char;
typedef uint32_t Utf32Char;

class StringConversionException : std::exception
{
};

class String
{
private:
    std::basic_string<Char> characters;
public:
    String() = default;

    String(const char *s) : characters(utf16(s))
    {

    }

    String(const std::string &s)
            : characters(Encoding::Utf8ToUtf16(std::basic_string<uint8_t>(s.begin(), s.end())))
    {

    }

    friend std::ostream &operator<<(std::ostream &stream, const String &s)
    {
        auto utf8String = Encoding::Utf16ToUtf8(s.characters);
        for (const auto &c : utf8String)
        {
            stream << (char) c;
        }
        return stream;
    }

    inline size_t size() const
    {
        return characters.size();
    }

    inline std::basic_string<Char>::iterator begin()
    {
        return characters.begin();
    }

    inline std::basic_string<Char>::iterator end()
    {
        return characters.end();
    }

    inline std::basic_string<Char>::const_iterator begin() const
    {
        return characters.begin();
    }

    inline std::basic_string<Char>::const_iterator end() const
    {
        return characters.end();
    }

    inline bool empty() const
    {
        return characters.empty();
    }

    inline Char &operator[](size_t index)
    {
        return characters[index];
    }

    inline const Char &operator[](size_t index) const
    {
        return characters[index];
    }

    inline Char &at(size_t index)
    {
        return characters.at(index);
    }

    inline const Char &at(size_t index) const
    {
        return characters.at(index);
    }

    static int32_t ParseInt(const String &s)
    {
        auto utf8String = Encoding::Utf16ToUtf8(s.characters);
        if (utf8String.size() != s.size())
        {
            throw StringConversionException();
        }
        else
        {
            return std::stoi(std::string(utf8String.begin(), utf8String.end()));
        }
    }

    static double_t ParseDouble(const String &s)
    {
        auto utf8String = Encoding::Utf16ToUtf8(s.characters);
        if (utf8String.size() != s.size())
        {
            throw StringConversionException();
        }
        else
        {
            return std::stod(std::string(utf8String.begin(), utf8String.end()));
        }
    }

    static int64_t ParseLong(const String &s)
    {
        auto utf8String = Encoding::Utf16ToUtf8(s.characters);
        if (utf8String.size() != s.size())
        {
            throw StringConversionException();
        }
        else
        {
            return std::stol(std::string(utf8String.begin(), utf8String.end()));
        }
    }

    static bool ParseBool(const String &s)
    {
        if (s.size() == 4 && s[0] == 't' && s[1] == 'r' && s[2] == 'u' && s[3] == 'e')
        {
            return true;
        }
        else if (s.size() == 5 && s[0] == 'f' && s[1] == 'a' && s[2] == 'l' && s[3] == 's' && s[4] == 'e')
        {
            return false;
        }
        else
        {
            throw StringConversionException();
        }
    }

    inline static String ToString(int32_t value)
    {
        return std::to_string(value);
    }

    inline static String ToString(double_t value)
    {
        return std::to_string(value);
    }

    inline static String ToString(int64_t value)
    {
        return std::to_string(value);
    }

    inline static String ToString(bool value)
    {
        return value ? "true" : "false";
    }

    inline static String ToString(Char c)
    {
        String s;
        s.push_back(c);
        return s;
    }

    inline void push_back(Char c)
    {
        characters.push_back(c);
    }

    inline void clear()
    {
        characters.clear();
    }

    inline size_t find(Char c)
    {
        return characters.find(c);
    }

    static constexpr auto npos{static_cast<size_t>(-1)};

    String &operator+=(const String &s)
    {
        characters.insert(characters.end(), s.begin(), s.end());
        return *this;
    }

    friend String operator+(String lhs, const String &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    inline friend bool operator==(const String &lhs, const String &rhs)
    {
        return lhs.characters == rhs.characters;
    }

    inline friend bool operator!=(const String &lhs, const String &rhs)
    {
        return lhs.characters != rhs.characters;
    }

    template<typename InputIterator>
    static String Join(String separator, InputIterator first, InputIterator last)
    {
        if (first == last)
        {
            return "";
        }
        else
        {
            String result = *first;
            first++;
            while (first != last)
            {
                result += separator;
                result += *first;
                first++;
            }
            return result;
        }
    }

    std::string ToCppString()
    {
        auto utf8String = Encoding::Utf16ToUtf8(characters);
        return std::string(utf8String.begin(), utf8String.end());
    }
};

template<>
struct std::hash<String>
{
    std::size_t operator()(const String &s) const
    {
        int hash = 0;
        if (!s.empty())
        {
            int n = s.size();
            for (int i = 0; i < n; i++)
            {
                hash = 31 * hash + s[i];
            }
        }
        return hash;
    }
};

#endif // STRING_HPP