#pragma once
#include "strategy.h"
#include <unordered_map>

template<class Key>
class AccessStrategy final : public Strategy<Key>
{
	using UMapIter = typename std::unordered_map<Key, int>::iterator;

	int hp; //количество обращений
	std::unordered_map<Key, int> KeysAccess;

public:
	AccessStrategy(int _hp) : hp(_hp) {}

	bool insert(const Key& key) override
	{
		std::pair<UMapIter, bool>tmp = KeysAccess.insert(std::pair<const Key, int>(key, 0));
		return tmp.second;
	}

	void erase(const Key& key) override
	{
		KeysAccess.erase(key);
	}

	bool access(const Key& key) override //можно ли обратиться или нет
	{
		if (KeysAccess.find(key) != KeysAccess.end() && KeysAccess[key] != hp)
		{
			++KeysAccess[key];
			return true;
		}
		return false;

	}

	bool check(const Key& key) override
	{
		return ((KeysAccess.find(key) != KeysAccess.end()) && (KeysAccess[key] != hp));
	}
};
