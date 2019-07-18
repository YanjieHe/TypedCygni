using System;
using System.Text;
using System.Collections.Generic;
namespace Compiler
{
    public abstract class JsonObject
    {
    }

    public class JsonArray : JsonObject
    {
        public List<JsonObject> items;
        public JsonArray(List<JsonObject> items) { this.items = items; }
    }
    public class JsonMap : JsonObject
    {
        public Dictionary<String, JsonObject> map;
        public JsonMap(Dictionary<String, JsonObject> map)
        {
            this.map = map;
        }

    }

    public class JsonValue : JsonObject
    {
        public String value;
        public JsonValue(String value)
        {
            this.value = value;
        }
    }

    public class JsonViewer
    {
        public JsonViewer()
        {

        }

        public String Visit(JsonObject json)
        {
            StringBuilder builder = new StringBuilder();
            Visit(json, builder);
            return builder.ToString();
        }

        void Visit(JsonObject json, StringBuilder builder)
        {
            if (json is JsonArray)
            {
                VisitArray((JsonArray)json, builder);
            }
            else if (json is JsonMap)
            {
                VisitMap((JsonMap)json, builder);
            }
            else if (json is JsonValue)
            {
                VisitValue((JsonValue)json, builder);
            }
            else
            {
                throw new NotSupportedException();
            }
        }

        void VisitArray(JsonArray array, StringBuilder builder)
        {
            bool first = true;
            builder.Append("[");
            foreach (var item in array.items)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    builder.Append(",");
                }
                Visit(item, builder);
            }
            builder.Append("]");
        }

        void VisitMap(JsonMap map, StringBuilder builder)
        {
            bool first = true;
            builder.Append("{");
            foreach (String key in map.map.Keys)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    builder.Append(",");
                }
                builder.AppendFormat("\"{0}\": ", key);
                Visit(map.map[key], builder);
            }
            builder.Append("}");
        }

        void VisitValue(JsonValue value, StringBuilder builder)
        {
            builder.AppendFormat("\"{0}\"", value.value);
        }
    }
}
