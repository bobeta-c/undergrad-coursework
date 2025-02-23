#include <iostream>
using namespace std;
void changeToZero(double& x);
int main(void){
	double y = 12;
	changeToZero(y);
	cout.setf(ios::fixed);
	cout.precision(1);
	double& b = y;
	cout << y << endl;
	cout << b << endl;
	y = 13;
	cout << y << endl;
	changeToZero(b);
	cout << y << endl;
	cout << &y << ' ' << &b << endl;
}
void changeToZero(double& x){
	x = 0;
	return;
}
