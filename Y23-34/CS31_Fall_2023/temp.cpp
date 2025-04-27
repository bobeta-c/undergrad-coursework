#include <iostream>
using namespace std;
int func(float n){
	return n*n;
}
int main(void){
	long long a = 10;
	int b = func(a);
	cout << b << endl;
}

