#include <iostream>
#include <string>
using namespace std;

int main(void){
	
	//get data and check for validity
	string identification;
	cout << "Identification: ";
	getline(cin, identification);
	if(identification == ""){
		cout << "---\nYou must enter a property identification.\n";
		return 1;
	}
	
	double overstatedAmount;
	cout << "Overstated amount (in millions): ";
	cin >> overstatedAmount;
	cin.ignore(10000, '\n');
	if (overstatedAmount <= 0){
		cout << "---\nThe overstated amount must be positive.\n";
		return 1;
	}
	
	string location;
	cout << "Location: ";
	getline(cin, location);
	if(location == ""){
		cout << "---\nYou must enter a location.\n";
		return 1;
	}

	//calculate fine
	double fine = 0;
	if(overstatedAmount > 150){
		fine += (overstatedAmount-150)*0.21;
		overstatedAmount = 150;
	}
	if(overstatedAmount > 60){
		if(location == "new york" || location == "florida"){
			fine += (overstatedAmount-60)*0.18;
		}
		else{
			fine += (overstatedAmount-60)*0.153;
		}
		overstatedAmount = 60;
	}
	fine += overstatedAmount*0.1111;

	//print fine
	cout.setf(ios::fixed);
	cout.precision(3);
	cout << "---\nThe fine for " << identification << " is $" << fine << " million.\n";
	return 0;
	

}
