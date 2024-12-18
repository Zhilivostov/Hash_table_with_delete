#pragma once
#include <unordered_map>
#include "Strategy.h"

template <class Key, class Value>
class HashTable
{
private:

	Strategy<Key>& strategy;
	std::unordered_map<Key, Value> umap;

	friend class HashTableIter;
	class HashTableIter
	{
		friend HashTable;
		using Iterator = typename std::unordered_map<Key, Value>::iterator;

	private:

		HashTable& parent;
		Iterator iter;

	public:

		using reference = typename HashTable::value_type&;

		HashTableIter(HashTable& _parent, Iterator _it) : parent{ _parent }, iter{ _it } {}

		HashTableIter& operator++ ()
		{
			++iter;
			while (iter != parent.umap.end())
			{
				Key key = (*iter).first;
				if (parent.strategy.check(key))
				{
					break;
				}
				++iter;
			}
			return *this;
		}

		reference operator*()
		{
			Key key = (*iter).first;
			bool access = parent.strategy.access(key);
			if (!access)
			{
				throw std::out_of_range("Invalid key");
			}

			return *iter;
		}

		bool operator!=(const HashTableIter& other) const
		{
			return iter != other.iter;
		}
	};

	HashTableIter getIterator(const Key& key)
	{
		if (umap.find(key) == umap.end())
			return HashTableIter(*this, umap.end());
		return HashTableIter(*this, umap.find(key));
	}

public:

	using UmapIter = typename std::unordered_map<Key, Value>::iterator;
	using value_type = std::pair<const Key, Value>;
	
	HashTable(Strategy<Key>& _strategy) : strategy(_strategy) { };
	~HashTable() = default;

	std::pair<HashTableIter, bool> insert(const Key& key, const Value& val)
	{
		if (!strategy.insert(std::move(key)) || umap.find(key) != umap.end())
		{
			throw std::invalid_argument("Such Key is already exist!");
		}
		auto tmp_pair = umap.insert(std::make_pair(key, val));
		return { HashTableIter(*this, tmp_pair.first), tmp_pair.second };
	}

	/*std::pair<HashTableIter, bool> insert(const Key& key, const Value& val)
	{
		if (umap.find(key) != umap.end())
			throw std::invalid_argument("Such Key is already exist!");

		bool success = strategy.insert(std::move(key)); ///5.
		if (success)
		{
			std::pair<UmapIter, bool> tmp_pair = umap.insert(std::make_pair(key, val));
			HashTableIter tmp(*this, tmp_pair.first);
			return std::pair<HashTableIter, bool>(tmp, tmp_pair.second);
		}
		return std::pair<HashTableIter, bool>(getIterator(key), false);
	}*/

	size_t erase(const Key& key)
	{
		if (umap.find(key) == umap.end())
			throw std::invalid_argument("Invalid key");

		strategy.erase(key);
		return umap.erase(key);
	}

	Value& operator[](const Key& key)
	{
		if (!strategy.access(key))
		{
			if (umap.find(key) != umap.end())
			{
				strategy.erase(key);
				erase(key);
			}
			bool success = strategy.insert(std::move(key)); ///5.
			if (!success)
			{
				throw std::out_of_range("Can't insert the key");
			}
		}
		return umap[key];
	}

	Value& at(const Key& key) //без const из-за erase
	{
		if (umap.find(key) == umap.end())
			throw std::invalid_argument("Invalid key");

		if (!strategy.access(key))
		{
			erase(key);
			throw std::out_of_range("key is expired");
		}
		return umap.at(key);
	}

	HashTableIter begin()
	{
		HashTableIter tmp(*this, umap.begin());
		return tmp;
	}

	HashTableIter end()
	{
		HashTableIter tmp(*this, umap.end());
		return tmp;
	}

};


//По второму блоку :
//1. friend class HashTable; внутри HashTableIter, это может привести к путанице.Если HashTableIter не должен быть доступен другим классам, лучше убрать это объявление.Если доступ нужен только для HashTable, то лучше явно указать это в HashTable.
//2. Метод getIterator должен проверять, существует ли ключ в umap, прежде чем возвращать итератор.Если ключ не найден, лучше вернуть итератор на end().
//3.
//
//C++
//std::pair<HashTableIter, bool> insert(const Key& key, const Value& val)
//{
//	if (!strategy.insert(key) || umap.find(key) != umap.end())
//	{
//		throw std::invalid_argument("Such Key is already exist!");
//	}
//	auto tmp_pair = umap.insert(std::make_pair(key, val));
//	return { HashTableIter(*this, tmp_pair.first), tmp_pair.second };
//}
//
//Лучше перепиши этод метод так.
//4.
//int hp - лучше size_t.
//5.
//
//C++
//bool insert(const Key & key) override
//{
//
//}
//
//Хочу чтобы эти функции имели перегрузку :
//
//C
//template <class Key>
//bool insert(Key&& key) {
//}
//
//Чтобы нормально обрабатывать rvalue ключи


//1. 
//2. вроде бы исправил, если правильно понял претензию.
//3. переписано!
//4. исправлено!
//5. исправлено!