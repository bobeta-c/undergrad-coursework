#include <iostream>
#include <string>
using namespace std;
const int strlen = 100;
void show_string(char* s, int l);
int main(void)
{
	char* number;
	number = (char*) calloc(strlen, sizeof(char));
	*(number+strlen-1) = 50;
	show_string(number, strlen);
	cout << "Enter a number: ";
	cin >> number;
	show_string(number, strlen);
	int sum = 0;
	for(int i = 0; *(number+i) != '\0'; i++){
		sum += *(number+i) - '0';
	}
	cout << sum << endl;
	free(number);
}
void show_string(char* s, int l){
	for(int i = 0; i < l*2; i++){
		cout << *(s+i) << ' ' << (int) *(s+i) << endl;
	}
	cout << endl;
	for(int i = 0; i < l*2; i++){
		cout << (int) *(s+i)<< ' ';
	}
	cout << endl << endl;
}
