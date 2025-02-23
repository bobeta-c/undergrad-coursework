#include <iostream>
using namespace std;

int main(void){
	double* cat;
	double mouse[5];
	cat = mouse+4;
	*cat = 25;
	*(mouse+3)=17;
	cat -= 3;
	cat[1]  = 42;
	cat[0] = 54;
	bool d = (cat == mouse);
	bool b = *cat == *(cat+1);
	cout << d << ' ' << b << " : " << mouse[0] << ' ' << mouse[1] << ' ' << mouse[2] << ' ' << mouse[3] << ' ' << mouse[4] << endl;
}
