using System;
using System.Text;
using System.Collections.Generic;
namespace Compiler
{
    public class Scope
    {
        public Scope parent;
        private Dictionary<String, Dictionary<String, Object>> table;
        public Scope()
        {
            this.parent = null;
            this.table = new Dictionary<string, Dictionary<string, object>>();
        }
        public Scope(Scope parent)
        {
            this.parent = parent;
            this.table = new Dictionary<string, Dictionary<string, object>>();
        }
        public Object Lookup(String name, String type)
        {
            Object result = LookupInLocal(name, type);
            if (result == null)
            {
                if (parent == null)
                {
                    return null;
                }
                else
                {
                    return parent.Lookup(name, type);
                }
            }
            else
            {
                return result;
            }
        }

        public Object LookupInLocal(String name, String type)
        {
            if (table.ContainsKey(name))
            {
                if (table[name].ContainsKey(type))
                {
                    return table[name][type];
                }
            }
            return null;
        }

        public void Insert(String name, String type, Object value)
        {
            if (table.ContainsKey(name))
            {
                table[name].Add(type, value);
            }
            else
            {
                table[name] = new Dictionary<string, object>
                {
                    [type] = value
                };
            }
        }

        public void Update(String name, String type, Object value)
        {
            if (table.ContainsKey(name))
            {
                if (table[name].ContainsKey(type))
                {
                    table[name][type] = value;
                }
                else
                {
                    throw new KeyNotFoundException(String.Format("<{0}, {1}>", name, type));
                }
            }
            else
            {
                throw new KeyNotFoundException(String.Format("<{0}, {1}>", name, type));
            }
        }

        public static Scope BuiltIn()
        {
            Scope scope = new Scope();

            scope.Insert("Void", "TYPE", Type.VOID);
            scope.Insert("Bool", "TYPE", Type.BOOL);
            scope.Insert("Int", "TYPE", Type.INT);
            scope.Insert("Float", "TYPE", Type.FLOAT);
            scope.Insert("Long", "TYPE", Type.LONG);
            scope.Insert("Double", "TYPE", Type.DOUBLE);
            scope.Insert("String", "TYPE", Type.STRING);

            return scope;
        }

        public Scope Clone()
        {
            Scope copy = new Scope(this.parent);
            foreach (var name in table.Keys)
            {
                var dictionary = table[name];
                foreach (var type in dictionary.Keys)
                {
                    copy.Insert(name, type, dictionary[type]);
                }
            }
            return copy;
        }

        public void ChangeParent(Scope parent)
        {
            this.parent = parent;
        }

        public override string ToString()
        {
            StringBuilder builder = new StringBuilder();
            foreach (var name in table.Keys)
            {
                var dictionary = table[name];
                foreach (var type in dictionary.Keys)
                {
                    if (builder.Length != 0)
                    {
                        builder.AppendLine();
                    }
                    builder.AppendFormat("<{0}, {1}> = {2}", name, type, dictionary[type]);
                }
            }
            return builder.ToString();
        }
    }
}
