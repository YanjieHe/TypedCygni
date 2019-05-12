#include "JSON.hpp"

void JsonToFile(const Ptr<JsonObject> &obj, std::ofstream &stream)
{
    static String colon = ":";
    static String LBracket = "[";
    static String RBracket = "]";
    static String LBrace = "{";
    static String RBrace = "}";
    static String comma = ",";

    auto quotationMarks = [](const String &str)
    {// TO DO: escaped characters
        static String mark = "\"";
        return mark + str + mark;
    };
    switch (obj->jsonType)
    {
        case JsonType::JsonValueType:
        {
            stream << quotationMarks(Cast<JsonValue>(obj)->value);
            break;
        }
        case JsonType::JsonArrayType:
        {
            stream << LBracket;
            auto array = Cast<JsonArray>(obj)->items;
            bool first = true;
            for (const auto &item: array)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    stream << comma;
                }
                JsonToFile(item, stream);
            }
            stream << RBracket;
            break;
        }
        case JsonType::JsonMapType:
        {
            stream << LBrace;
            auto map = Cast<JsonMap>(obj)->map;
            bool first = true;
            for (const auto &pair: map)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    stream << comma;
                }

                stream << quotationMarks(pair.first);
                stream << colon;
                JsonToFile(pair.second, stream);
            }
            stream << RBrace;
            break;
        }
    }
}
