#include <iostream>
using namespace std;
void swap2(int* a, int *b)
{
 int temp = *a; *a = *b; *b = temp;
}
int main()
{
int arr[7] = { 2,4,6,8,1,3,5 };
int* first = arr;
 int* last = arr+6;
 int divider = 4;
 while(first < last) {
 while( (first < last) && (*first < divider) ) first++;
 cout << first-arr;
 while( (first < last) && (*last > divider) ) last--;
 cout << last-arr;
 swap2(first,last);
 }
 for(first = arr ; first < arr+7; first++)
 cout << *first << " ";
 cout << endl;
}
