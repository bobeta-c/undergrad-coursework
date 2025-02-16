#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <string>
#include <iostream>


const int DEFAULT_BUCKET_SIZE = 10;

template <typename T>
class HashMap
{
public:
	HashMap(double max_load = 0.75); // constructor
	~HashMap(); // destructor; deletes all of the items in the hashmap
	int size() const {return m_size;} // return the number of associations in the hashmap
	// The insert method associates one item (key) with another (value).
	// If no association currently exists with that key, this method inserts
	// a new association into the hashmap with that key/value pair. If there is
	// already an association with that key in the hashmap, then the item
	// associated with that key is replaced by the second parameter (value).
	// Thus, the hashmap must contain no duplicate keys.
	void insert(const std::string& key, const T& value);
	// If no association exists with the given key, return nullptr; otherwise,
	// return a pointer to the value associated with that key. This pointer can be
	// used to examine that value or modify it directly within the map.
	const T* find(const std::string& key) const;
	// Defines the bracket operator for HashMap, so you can use your map like this:
	// your_map["david"] = 2.99;
	// If the key does not exist in the hashmap, this will create a new entry in
	// the hashmap and map it to the default value of type T. Then it will return a
	// reference to the newly created value in the map.
	T& operator[](const std::string& key);
	T* find(const std::string& key) {
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}
	HashMap(const HashMap&) = delete;
	HashMap& operator=(const HashMap&) = delete;
private:
	void resize(int new_size);
	struct Node
	{
		T value{};
		std::string key{};
	};
	int m_size;
	double m_max_load;
	std::vector<std::list<Node>> m_buckets; 
}; 

template <typename T>
HashMap<T>::HashMap(double max_load)
	: m_size(0), m_max_load(max_load), m_buckets(DEFAULT_BUCKET_SIZE)
{
	if(m_max_load <= 0)
		m_max_load = 0.75;

	//cerr << "made" << endl;
}

template <typename T>
HashMap<T>::~HashMap()
{}

template <typename T>
void HashMap<T>::resize(int new_size){
	std::vector<std::list<Node>> new_buckets(new_size);

	for(int i = 0; i < m_buckets.size(); i++){
		while(m_buckets[i].size() > 0){
			typename std::list<typename HashMap<T>::Node>::iterator p = m_buckets[i].begin();
			size_t index = std::hash<std::string>()((*p).key) % new_buckets.size();
			new_buckets[index].splice(new_buckets[index].end(), m_buckets[i], p);
		}
	}

	m_buckets.swap(new_buckets);
	//cerr << "resized with new size " << m_buckets.size() << " and current size " << m_size << endl;
}

template <typename T>
void HashMap<T>::insert(const std::string& key, const T& value)
{
	
	T* isAt = find(key);
	if(isAt == nullptr){
		size_t index = std::hash<std::string>()(key) % m_buckets.size();
		Node n;
		n.value = value;
		n.key = key;

		m_buckets.at(index).push_back(n);
		m_size++;
		double m_current_load = (0.0+size())/m_buckets.size();
		if(m_current_load > m_max_load){
			resize(2*m_buckets.size());
		}
	}
	else{
		*isAt = value;
	}

}


template <typename T>
const T* HashMap<T>::find(const std::string& key) const{
	size_t index = std::hash<std::string>()(key) % m_buckets.size();

	for(typename std::list<typename HashMap<T>::Node>::const_iterator p = m_buckets[index].begin(); p != m_buckets[index].end(); p++){
		if((*p).key == key){
			return &(*p).value;
		}
	}
	return nullptr;
}

template <typename T>
T& HashMap<T>::operator[](const std::string& key){
	T* atKey = find(key);
	if(atKey != nullptr){
		return (*atKey);
	}
	T value{};
	insert(key, value);
	return (*find(key));
}



#endif
