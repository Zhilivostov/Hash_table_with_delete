#pragma once
#include "strategy.h"
#include <ctime>
#include <unordered_map>

template<class Key>
class TimeStrategy final : public Strategy<Key>
{
	using UMapIter = typename std::unordered_map<Key, std::time_t>::iterator;

	std::time_t time; //������� ����
	std::unordered_map<Key, std::time_t> KeysTime;

public:
	TimeStrategy(std::time_t _timeout) : time(_timeout) {}
	
	bool insert(const Key& key) override
	{
		std::time_t currentTime = std::time(nullptr); //���������� ������� �����
		std::pair<UMapIter, bool> tmp = KeysTime.insert(std::pair<const Key, std::time_t>(key, currentTime));
		return tmp.second;
	}

	void erase(const Key& key) override
	{
		KeysTime.erase(key);
	}

	bool access(const Key& key) override //����� ���������� ��� ���
	{
		if (KeysTime.find(key) == KeysTime.end())
		{
			return false;
		}
		std::time_t lifeTime = std::time(nullptr) - KeysTime[key]; //������ �� ����� ��� ��������
		return lifeTime < time; //���� true, ������ ��� �����
	}

	bool check(const Key& key) override
	{
		return ((KeysTime.find(key) != KeysTime.end()) && (KeysTime[key] < time));
	}

};