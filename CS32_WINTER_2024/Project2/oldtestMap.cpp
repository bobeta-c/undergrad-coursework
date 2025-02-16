#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Map m1;
	Map m2;
	for(int i = 0; i < 100; i++){
		m1.insert(to_string(i),i);
		m2.insert(to_string(-i),-i);
	}

	assert(m1.insert("Fred", 123));
	assert(m1.insert("Ethel", 456));
	assert(m1.size() == 102);
	ValueType v = 42;
	assert(!m1.get("Lucy", v)  &&  v == 42);
	assert(m1.get("Fred", v)  &&  v == 123);
	Map m3;
	reassign(m1, m3);
	for(int i = 0; i < 100; i++){
		KeyType key = "noKey";
		ValueType value = 0;
		KeyType k1;
		ValueType v1;
		m1.get(i, k1, v1);
		m3.get(i, key, value);
		cout << "m3: " << key << " " << value << " m1: " << k1 << ' ' << v1 << endl;
	}
	merge(m1,m2,m3);
	for(int i = 0; i < 209; i++){
		KeyType key = "noKey";
		ValueType value = 0;
		KeyType k1;
		ValueType v1;
		m1.get(i, k1, v1);
		m3.get(i, key, value);
		cout << "m3: " << key << " " << value << " m1: " << k1 << ' ' << v1 << endl;
	}

}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
