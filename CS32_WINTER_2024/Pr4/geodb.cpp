#include "geodb.h"
#include <string>
#include <iostream>
#include <fstream>
#include "geotools.h"

using namespace std;

GeoDatabase::GeoDatabase(){	 //constructor
}

GeoDatabase::~GeoDatabase(){ //destructor
}


bool GeoDatabase::load(const std::string& map_data_file)
{
	ifstream infile(map_data_file);
	if(!infile){
		cerr << "Cannot open data text" << endl; // error message
		return false;
	}
	string line;
		while(getline(infile, line))
	{
		//parse data from each file
		string loc_name = line;
		string x1, x2, y1, y2;
		infile >> x1;
		infile >> y1;
		infile >> x2;
		infile >> y2;
		infile.ignore(1000, '\n');
		Data g1, g2;
		g1.location_name = loc_name;
		g1.end_location = GeoPoint(x1, y1); //start of street
		g2.location_name = loc_name;
		g2.end_location = GeoPoint(x2,y2); //end of street
		m_connections[g1.end_location.to_string()].push_back(g2); //make connection between start of street and end of street
		m_connections[g2.end_location.to_string()].push_back(g1); //make a connection between end of street and start of street
		m_edge_names[orderTwo(g1.end_location, g2.end_location)] = loc_name;
		int pois;
		infile >> pois; // number of points of interest
		infile.ignore(1000, '\n');
		if(pois > 0){
			Data mid;
			mid.location_name = loc_name;
			mid.end_location = midpoint(g2.end_location, g1.end_location);
			m_connections[g1.end_location.to_string()].push_back(mid);
			m_connections[mid.end_location.to_string()].push_back(g1);
			m_connections[mid.end_location.to_string()].push_back(g2);
			m_connections[g2.end_location.to_string()].push_back(mid);
			m_edge_names[orderTwo(g1.end_location, mid.end_location)] = loc_name; // input the street names redundantly
			m_edge_names[orderTwo(mid.end_location, g2.end_location)] = loc_name;
			//Make connections between each start to the midpoint
			for(int i = 0; i < pois; i++){
				string name;
				char c;
				while(infile.get(c)){
					if(c == '|')
						break;
					name.push_back(c);
					if(c == '\n'){
						cerr << "something goofy happened" << endl;
						return false;
					}
				}
				// here is where I would change g1 and mid names to path if this were a path
				g1.location_name = "a path";
				mid.location_name = "a path";
		        infile >> x1;
		        infile >> y1;
				// make g1 the location of the poi
				g1.end_location = GeoPoint(x1, y1);
				m_connections[g1.end_location.to_string()].push_back(mid);
				m_connections[mid.end_location.to_string()].push_back(g1);
				m_edge_names[orderTwo(g1.end_location, mid.end_location)] = "a path";
				m_pois[name] = g1.end_location;
				infile.ignore(1000, '\n');
			}
		}
	}
	return true;
}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	const GeoPoint* test = m_pois.find(poi); //just checks the poi hashmap
	if(test != nullptr){
		point = *test;
		return true;
	}
	return false;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
	vector<GeoPoint> points; //checks the connected points array
	if(m_connections.find(pt.to_string()) == nullptr) return points; // make sure it is a valid geopoint
	for(int i = 0; i < m_connections.find(pt.to_string())->size(); i++){
		points.push_back((*m_connections.find(pt.to_string()))[i].end_location); // add every connected geoPoint
	}
	return points;
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	const string* name = m_edge_names.find(orderTwo(pt1, pt2)); // this could have been implemented using the get_connected_points db but that is not O(1) it is O(V) where V is the amount of connected vertices
	if(name != nullptr){
		return *name; // just check the m_edge_names hashmap
	} 
	return string();
}

std::string GeoDatabase::orderTwo(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	if(pt1.to_string() < pt2.to_string()){ //used for indexing into the m_edge_names hashmap.
		return pt1.to_string()+pt2.to_string();
	}
	return pt2.to_string()+pt1.to_string();
}
