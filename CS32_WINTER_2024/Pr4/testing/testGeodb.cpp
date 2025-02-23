#include "geodb.h"
#include <iostream>
using namespace std;

int main() {	
	GeoDatabase g;
	g.load("mapdata.txt"); // assume this works to avoid error checking
	GeoPoint p1("34.0602175", "-118.4464952");
	GeoPoint p2("34.0602175", "-118.4464952");
	cout << g.get_street_name(p1, p2); // writes "Kinross Avenue"
	cout << g.get_street_name(p2, p1); // writes "Kinross Avenue"
}