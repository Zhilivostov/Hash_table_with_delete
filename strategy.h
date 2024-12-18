#pragma once

template<class Key>
class Strategy
{
public:
	virtual ~Strategy() = default;
	virtual bool insert(const Key&& key) = 0; //вставка элемента
	virtual void erase(const Key& key) = 0; //удаление элемента
	virtual bool access(const Key& key) = 0; //обращение к элементу	
	virtual bool check(const Key& key) = 0; //занят ли ключ 

};
