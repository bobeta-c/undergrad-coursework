#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Map m; //default constructor
	assert(m.size() == 0); //size is accurate
	assert(m.empty());
//	assert(m.erase("don't erase me") == false) //doesnt erase non-member function
	assert(m.size() == 0); //doesnt change size
	KeyType k;
	ValueType v;
	assert(!(m.get(0,k,v))); //cant get something you dont have
	k = "vik";
	v =  1;
	assert(m.insert(k,v)); //can insert
	assert(m.insert("roe", 2));
	assert(m.insert("joe", 3));
	assert(m.size() == 3); //updates size
	assert(m.update("vik", 4));
	assert(m.insertOrUpdate("sim", 1)); //works
	assert(!m.empty()); // isnt empty
	Map m2;
	m2 = m; // assignment operator
	Map m3 = m; //copy constructor
	assert(m2.size() == m.size() && m3.size() == m.size()); //they should all be equal
	assert(m.erase("vik")); // one should be smaller
	assert(m.size() == m2.size()-1); // check
	reassign(m, m2); //see if this works
	assert(m2.size() == m.size()); // they have the same size thats good
	KeyType k2;
	ValueType v2;
	for(int i = 0; i < m.size(); i++){
		m.get(i, k, v);
		m2.get(i, k2, v2);
		assert((k == k2) && (v != v2)); //make sure that they have the same keys but different values
	}
	assert(!merge(m2, m3, m)); //merging m2 and m3 should make issues because m3 and m2 have the same keys but diffferent values

}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
