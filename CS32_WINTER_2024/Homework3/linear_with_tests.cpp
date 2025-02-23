#include <iostream>
using namespace std;
bool somePredicate(double x){
	return x > 0;
}
// Return true if the somePredicate function returns false for at
// least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
	if(n <= 0){
	       return false;
	}
	if(!somePredicate(a[0])) return true;
	return anyFalse(a+1, n-1);	
}

  // Return the number of elements in the array for which the
  // somePredicate function returns false.
int countFalse(const double a[], int n)
{
	if(n <= 0){
		return 0;
	}
	return !somePredicate(a[0]) + countFalse(a+1,n-1);
}

  // Return the subscript of the first element in the array for which
  // the somePredicate function returns false.  If there is no such
  // element, return -1.
int firstFalse(const double a[], int n)
{
	if(n<=0){
		return -1;
	}
	if(!somePredicate(a[0])){
		return 0;
	}
	int next = firstFalse(a+1,n-1);
	if(next == -1){
		return -1;
	}
	return 1+next;
}

  // Return the subscript of the first double in the array that is <=
  // all doubles in the array (i.e., return the smallest subscript m such
  // that a[m] <= a[k] for all k from 0 to n-1).  If the function is
  // told that no doubles are to be considered to be in the array,
  // return -1.
int locateMinimum(const double a[], int n)
{
	if(n <= 0){
		return -1;
	}
	if(n == 1){
		return 0;
	}
	int halfway = n/2;
	int x = locateMinimum(a, halfway);
	int y = locateMinimum(a+halfway, n-halfway) + halfway;
	//cout << "comparing" << endl;
	if(a[x] <= a[y]){
		return x;
	}
	return y;

}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not include
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if(n2 == 0){
		return true;
	}
	if(n1 == 0){
		return false;
	}
	cout << "including!"<<endl;
	if(a1[0] == a2[0]){
		return includes(a1+1,n1-1,a2+1,n2-1);
	}
	return includes(a1+1,n1-1,a2,n2);
}

int main(void){
	double a[] = {1,2,3,4,5,6,7,8,9,10};
	cout << anyFalse(a,10) << endl;
	a[9] = -10;
	cout << anyFalse(a,10) << endl;
	a[1] = -2;
	a[5] = -6;
	cout << countFalse(a,10) << endl;
	cout << firstFalse(a,10) << endl;
	a[1] = 2;
	cout << firstFalse(a,10) << endl;
	a[5] = 6;
	a[9] = 10;
	cout << firstFalse(a,10) << endl;
	cout << countFalse(a,10) << endl;
	cout << locateMinimum(a,10) << endl;
	a[7] = -8;
	cout << locateMinimum(a,10) << endl;
	double a1[] = {10,50,40,20,50,40,30};
	double a2[] = {50,20,30};
	cout << includes(a1,7,a2,3) << endl;
	double a3[] = {50,30,20};
	cout << includes(a1,7,a3,3) << endl;


	double b[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,1,2,3,4,5,6,7,8,9,-30,1,2,3,4,5,6,7,8,9,40,1,2,3,4,5,6,7,8,9,50,1,2,3,4,5,6,7,8,9,60};
	cout << locateMinimum(b,60) << endl;
}

