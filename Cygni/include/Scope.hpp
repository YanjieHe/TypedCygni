#ifndef CYGNI_SCOPE_HPP
#define CYGNI_SCOPE_HPP
#include "Utility.hpp"
#include <any>
#include <memory>
#include <list>
#include <optional>

namespace cygni
{
	/*class Scope
	{
	public:
		std::shared_ptr<Scope> parent;
		Table<std::u32string, std::any> table;

		Scope() = default;

		explicit Scope(std::shared_ptr<Scope> parent);

		std::optional<std::any> Get(std::u32string key);

		void Put(std::u32string key, std::any value);

		std::optional<std::any> GetInLocal(std::u32string key);
	};

	using ScopePtr = std::shared_ptr<Scope>;*/

	template <typename T>
	class Scope
	{
	private:
		template <typename T>
		friend class ScopeFactory;
		Scope<T>* parent;
		std::unordered_map<std::u32string, T> table;

		Scope() :parent{ nullptr }
		{

		}
		explicit Scope(Scope<T>* parent) : parent{ parent }
		{
		}
	public:
		typename std::optional<T> Get(std::u32string key)
		{
			if (table.find(key) != table.end())
			{
				return std::optional<T>{ table[key] };
			}
			else
			{
				if (parent)
				{
					return parent->Get(key);
				}
				else
				{
					return std::optional<T>();
				}
			}
		}

		void Put(std::u32string key, const T& value)
		{
			table.insert({ key, value });
		}

		typename std::optional<T> GetInLocal(std::u32string key) const
		{
			if (table.find(key) != table.end())
			{
				return table[key];
			}
			else
			{
				return std::optional<T>();
			}
		}
	};

	template <typename T>
	class ScopeFactory
	{
	public:
		std::list<Scope<T>*> scopeList;

		ScopeFactory() : scopeList{}
		{

		}

		static typename std::shared_ptr<ScopeFactory<T>> Create()
		{
			return std::make_shared<ScopeFactory<T>>();
		}

		typename Scope<T>* New()
		{
			auto scope = new Scope<T>();
			scopeList.push_back(scope);
			return scope;
		}

		typename Scope<T>* New(Scope<T>* parent)
		{
			auto scope = new Scope<T>(parent);
			scopeList.push_back(scope);
			return scope;
		}

		~ScopeFactory()
		{
			for (auto scope : scopeList)
			{
				delete scope;
			}
			scopeList.clear();
		}
	};

} // namespace cygni

#endif // CYGNI_SCOPE_HPP
