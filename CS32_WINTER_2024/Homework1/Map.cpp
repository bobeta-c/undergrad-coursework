#include "Map.h"
#include <iostream>

Map::Map()
 : m_n_pairs(0)
{}

bool Map::empty() const
{
	return size() == 0;
}

int Map::size() const
{
	return m_n_pairs;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if(!contains(key) && (size() < DEFAULT_MAX_ITEMS)){
		m_pairs[size()].k = key;
		m_pairs[size()].v = value;
		m_n_pairs++;
		return true;
	}
	return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if(contains(key)){
		m_pairs[findLocation(key)].v = value;
		return true;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if(!insert(key, value)){
		if(!update(key, value)){
			return false;
		}
	}
	return true;
}

bool Map::erase(const KeyType& key)
{
	if(contains(key)){
		m_pairs[findLocation(key)] = m_pairs[size()-1];
		m_n_pairs--;
		return true;
	}
	return false;
}

bool Map::contains(const KeyType& key) const{
	return(findLocation(key) != -1);
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if(contains(key)){
		value = m_pairs[findLocation(key)].v;
		return true;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if(!(i >= 0 && i < size())){
		return false;
	}
	for( int j = 0; j < size(); j++){
		int count = 0;
		for(int h = 0; h < size(); h++){
			if(m_pairs[j].k > m_pairs[h].k){
				count++;
			}
		}
		if(count == i){
			key = m_pairs[j].k;
			value = m_pairs[j].v;
			return true;
		}
	}
	std::cerr << "Reached end of get(i,k,v) without finding the equality" << std::endl;
	return false;
}

void Map::swap(Map& other){
	for(int i = 0; i < DEFAULT_MAX_ITEMS; i++){
		Pair temp = other.m_pairs[i];
		other.m_pairs[i] = this->m_pairs[i];
		this->m_pairs[i] = temp;
	}
	int temp_size = other.size();
	other.m_n_pairs = this->size();
	this->m_n_pairs = temp_size;
}

int Map::findLocation(const KeyType& key) const
{
	for(int i = 0; i < size(); i++){
		if(m_pairs[i].k == key){
			return i;
		}
	}
	return -1;
}










