#include <iostream>
#include <string>
using namespace std;

int main(void)
{
	string number;
	cout << "Enter a number: ";
	cin >> number;
	int sum = 0;
	for(int i = 0; number[i] != '\0'; i++){
		sum += number[i]-'0';
	}
	cout << sum << endl;
}
