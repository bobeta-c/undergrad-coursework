#include "Map.h"
#include <iostream>

Map::Map()
 : m_head(nullptr), m_pairs(0)
{}

bool Map::empty() const
{
	return size() == 0;
}

int Map::size() const
{
	return m_pairs;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if(!contains(key)){
		Node* tempNode = new Node;
		tempNode->n_pair.k = key;
		tempNode->n_pair.v = value;
		if(m_head == nullptr){
			m_head = tempNode; // if no linked list set the head to point at our first key/value pair
		}
		else if(size() == 1){
			// if only one element in linked list set that element to point in both directions to our new node and set our new node to point in both directions to our tempNode
			m_head->n_next = tempNode;
			m_head->n_previous = tempNode;
			tempNode->n_next = m_head;
			tempNode->n_previous = m_head;
		}
		else{
			// if more than one new element in list set temp to point to where the first node is pointing (both forward and back)  and then set the previous one to point at it and the one in front to point at the tempNode back
			tempNode->n_next = m_head;
			tempNode->n_previous = m_head->n_previous;
			m_head->n_previous->n_next = tempNode;
			m_head->n_previous = tempNode;

		}
		m_pairs++;
		return true;
	}
	return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if(contains(key)){
		findLocation(key)->n_pair.v = value;
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

		Node* toDelete = findLocation(key);
		//findLocation returns pointer to the node with the key
		if(size() == 1){// if there is only one element delete it and set head to nullptr
			delete toDelete;
			m_head = nullptr;
		}
		else{ //if therer is more than one node then set the previous to point to next and next to point to previous and make sure that the head is not pointing to the poitner and then delete it
			if(m_head == toDelete){
				m_head = toDelete->n_next;
			}
			toDelete->n_next->n_previous = toDelete->n_previous;
			toDelete->n_previous->n_next = toDelete->n_next;
			delete toDelete;
		}
		m_pairs--;
		return true;
	}
	return false;
}

bool Map::contains(const KeyType& key) const{
	return(findLocation(key) != nullptr);
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if(contains(key)){
		value = findLocation(key)->n_pair.v;
		return true;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if(!(i >= 0 && i < size())){
		return false;
	}
	Node* it = m_head;
	do
	{// loop through each element in a do while loop where we start with an element and go until we return to the element
		int count = 0;
		Node* jt = m_head;
		do
		{//loop through every element again and make the comparison if the object we started with is strictly greater than
			if(it->n_pair.k > jt->n_pair.k){
				count++;
			}
			jt = jt->n_next;
		}
		while(jt != m_head);
		if(count == i){
			key = it->n_pair.k;
			value = it->n_pair.v;
			return true;
		}
		it = it->n_next;
	}
	while(it != m_head);

	std::cerr << "Reached end of get(i,k,v) without finding the equality" << std::endl;//Just a sanity check - there should never be the case where you reach the end of theloop
	return false;
}

void Map::swap(Map& other){

	int temp_size = other.m_pairs;
	other.m_pairs = this->m_pairs;
	this->m_pairs = temp_size;

	Node* temp_head = other.m_head;
	other.m_head = this->m_head;
	this->m_head = temp_head;
}

Map::~Map()
{
	if(size() == 0){
		//nothing to do
		return;
	}
	Node* iterator = m_head->n_next;
	while(iterator != m_head)
	{
		//delete the previous one until you are pointing to the one already deleted
		iterator = iterator->n_next;
		delete iterator->n_previous;
	}
	delete m_head;
}

Map::Map(const Map& other) : m_head(nullptr), m_pairs(0)
{
	//crucial detail (starting m_pairs = 0 instead of = other.m_pairs because insert operator increases m_pairs).
	for(int i = 0; i < other.size(); i++){// gets all items from other and inserts them to this
		KeyType key;
		ValueType value;
		other.get(i, key, value);
		insert(key, value);
	}
}

Map& Map::operator=(const Map& rhs)
{
	if(this != & rhs){
		Map temp(rhs);
		swap(temp);
	}
	return *this;
}

Map::Node* Map::findLocation(const KeyType& key) const
{
	if(size() == 0){
		return nullptr;
	}
	Node* iterator = m_head;
	do{
		//check every value until you find the key
		if(iterator->n_pair.k == key){
			return iterator;
		}
		iterator = iterator->n_next;
	}
	while(iterator != m_head && iterator != nullptr);


	return nullptr;
}


bool merge(const Map& m1, const Map& m2, Map& result)
{
	bool returnVal = true;
	result = m1; //set result to m1 and then proceed to go through every element of m2 and check if it disagrees with m1 in which case skip and set returnVal to false if it does not disagree then add it to the result
	for(int i = 0; i < m2.size(); i++){
		KeyType key;
		ValueType value;
		m2.get(i, key, value);
		if(m1.contains(key)){
			ValueType tempV;
			result.get(key, tempV);
			if(value != tempV){
				result.erase(key);
				returnVal = false;
			}
		}
		else{
			result.insert(key, value);
		}
	}
	return returnVal;
}



void reassign(const Map& m, Map& result)
{
	//set result equal to the first map
	//then for each value get the value in front and swap the two in order of how they appear in the linked list
	result = m;
	KeyType key1;
	ValueType value1;
	KeyType key2;
	ValueType value2;
	for(int i = 0; i < (m.size()-1); i++){
		m.get(i,key1,value1);
		m.get(i+1,key2,value2);
		result.update(key2,value1);
	}
	m.get(0,key1,value1);
	result.update(key1,value2);
}



