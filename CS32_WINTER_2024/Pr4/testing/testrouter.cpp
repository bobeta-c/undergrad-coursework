#include "router.h"
#include "geodb.h"
#include <iostream>
#include <string>


using namespace std;

int main(void)
{
	GeoDatabase g;
	g.load("mapdata.txt");
	Router r(g);
	GeoPoint p1, p2;
	g.get_poi_location("John Wooden Center", p1);
	g.get_poi_location("Diddy Riese", p2);
	GeoPoint p3("34.0650340", "-118.4453275");
	GeoPoint p4("34.0648327", "-118.4454041");
	std::vector<GeoPoint> points = r.route(p1, p3);
	for(int i = 0; i < points.size(); i++){
		if(i > 0)
			cout << g.get_street_name(points[i], points[i-1]) << " ";
		cout << points[i].sLatitude << " " << points[i].sLongitude << endl;
	}
}