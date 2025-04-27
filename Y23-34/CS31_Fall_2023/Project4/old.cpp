#include <iostream>
#include <string>
#include <cassert>
using namespace std;


int reduplicate(string a[], int n);
int locate(const string a[], int n, string target);
int locationOfMax(const string a[], int n);
int circleLeft(string a[], int n, int pos);
int enumerateRuns(const string a[], int n);
int flip(string a[], int n);
int locateDifference(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int locateAny(const string a1[], int n1, const string a2[], int n2);
int divide(string a[], int n, string divider);


int main()
{
	string c[7] = { "a", "b", "b", "d", "g", "g", "" };
	for(int i = 0; i < 7; i++){
		if(c[i] == ""){
			cout << 'l' << ' ';
		}
		cout << c[i] << ' ';
	}
	cout << endl;
	cout << divide(c, 2, "alpha")<< '\n';
	for(int i = 0; i < 7; i++){
		if(c[i] == ""){
			cout << "l ";
		}
		cout << c[i] << ' ';
	}
	cout << endl;
}

int reduplicate(string a[], int n){
	if(n < 0){
		return -1;
	}
	for(int i = 0; i < n; i++){
		a[i] = a[i]+a[i];
	}
	return n;
}
int locate(const string a[], int n, string target){
	if(n < 0){
		return -1;
	}
	for(int i = 0; i < n; i++){
		if(a[i] == target){
			return i;
		}
	}
	return -1;
}	
int locationOfMax(const string a[], int n){
	if(n <= 0){
		return -1;
	}
	string max = a[0];
	int pos = 0;
	for(int i = 1; i < n; i++){
		if(a[i] > max){
			pos = i;
			max = a[i];
		}
	}
	return pos;
}
int circleLeft(string a[], int n, int pos){
	if(n <= 0){
		return -1;
	}
	if(pos >= n){
		return -1;
	}
	string temp = a[pos];
	for(int i = pos; i < n-1; i++){
		a[i] = a[i+1];
	}
	a[n-1] = temp;
	return pos;
}
int enumerateRuns(const string a[], int n){
	if(n < 0){
		return -1;
	}
	if(n == 0){
		return 0;
	}
	int runs = 1;
	string cs = a[0];
	for(int i = 1; i < n; i++){
		if(a[i] != cs){
			cs = a[i];
			runs++;
		}
	}
	return runs;
}
int flip(string a[], int n){
	if(n < 0){
		return -1;
	}
	int halfway = n/2;
	for(int i = 0; i < halfway; i++){
		string temp = a[n-1-i];
		a[n-1-i] = a[i];
		a[i] = temp;
	}
	return n;
}
int locateDifference(const string a1[], int n1, const string a2[], int n2){
	if(n1 < 0 || n2 < 0){
		return -1;
	}
	for(int i = 0; i < n1 && i < n2; i++){
		if(a1[i] != a2[i]){
			return i;
		}
	}
	if(n1 <= n2){
		return n1;
	}
	return n2;
}
int subsequence(const string a1[], int n1, const string a2[], int n2){
	if(n1 < 0 || n2 < 0){
		return -1;
	}
	if(n2 == 0){
		return 0;
	}
	if(n1 == 0){
		return -1;
	}
	for(int i = 0; i < n1; i++){
		if(a1[i] == a2[0]){
			bool worked = true;
			for(int j = 1; j < n2; j++){
				if(!((j+i) < n1)){
					worked = false;
					break;
				}
				if(a1[j+i] != a2[j]){
					worked = false;
					break;
				}
			}
			if(worked){
				return i;
			}
		}
	}
	return -1;

}
int locateAny(const string a1[], int n1, const string a2[], int n2){
	if(n1 < 0 || n2 < 0){
		return -1;
	}
	for(int i = 0; i < n1; i++){
		for(int j = 0; j < n2; j++){
			if(a1[i] == a2[j]){
				return i;
			}
		}
	}
	return -1;
}
int divide(string a[], int n, string divider){
	if(n < 0){
		return -1;
	}
	int i = 0;
	int j = n-1;
	while(i <= j){
		if(a[i] >= divider){
			string temp = a[j];
			a[j] = a[i];
			a[i] = temp;
			j--;
		}
		else{
			i++;
		}
	}
	return i;
}

