#include <iostream>
using namespace std;

int* nochange(int* p)
{
	return p;
}

int* getPtrToArray(int arr[], int& m)
{
	int anArray[100];
	for (int j = 0; j < 100; j++)
	    arr[j] = 100-j;
	m = 100;
	return nochange(arr);
}

void f()
{
	int junk[100];
	for (int k = 0; k < 100; k++)
	    junk[k] = 123400000 + k;
	junk[50]++;
}

int main()
{
	int n;
	int arr[100];
	int* ptr = getPtrToArray(arr,n);
	f();
	for (int i = 0; i < 3; i++)
	    cout << ptr[i] << ' ';
	for (int i = n-3; i < n; i++)
	    cout << ptr[i] << ' ';
	cout << endl;
}
