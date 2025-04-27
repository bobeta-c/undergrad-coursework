#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>
#include "OurType.h"

using KeyType = OurType;
using ValueType = std::string;


const int DEFAULT_MAX_ITEMS = 150;

 
class Map
{
  public:
    Map();         // Create an empty map (i.e., one whose size() is 0).

    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that either the key is already in the map, or the map has a fixed
      // capacity and is full).

    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value that it currently maps to, but instead map to
      // the value of the second parameter; in this case, return true.
      // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value that it currently maps to, but instead map to
      // the value of the second parameter; in this case, return true.
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that the key is not already in the map and the map has a fixed
      // capacity and is full).

    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.
     
    bool contains(const KeyType& key) const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.
     
    bool get(const KeyType& key, ValueType& value) const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map which the key maps to, and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.
     
    bool get(int i, KeyType& key, ValueType& value) const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value of the key/value pair in the map whose key is strictly
      // greater than exactly i keys in the map and return true.  Otherwise,
      // leave the key and value parameters unchanged and return false.

    void swap(Map& other);
      // Exchange the contents of this map with the other one.
    ~Map();
      // Deallocate
     Map(const Map& other);
      //copy constructor
     Map& operator=(const Map& rhs);
      // = operator

  private:
    struct Pair {
	    KeyType k;
	    ValueType v;
    };

    struct Node {
	    Pair n_pair;
	    Node* n_next = this;
	    Node* n_previous = this;
    };

    Node* m_head;
    int m_pairs;
    
    Node* findLocation(const KeyType& key) const;
     // return node location of key value pair in m_pairs
     // return nullptr if not found

};

bool merge(const Map& m1, const Map& m2, Map& result);
	// If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
	//If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain exactly one pair with that key and value.

void reassign(const Map& m, Map& result);
	//do the fun swap


#endif
