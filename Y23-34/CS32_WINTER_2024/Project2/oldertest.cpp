#include "Map.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Map m1;
	Map m2;
	KeyType str = "test";
	ValueType v = 11;
	m1.insertOrUpdate(str, v);
	m1.insert("test1",10);
	m1.insert("test1",9);
	for(int i = 0; i < 2; i++)
	{
		m1.get(i,str,v);
		cout << str << ' ' << v << endl;
		cout << m1.size() << endl;
	}
	assert(m1.size() == 2);


}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}
