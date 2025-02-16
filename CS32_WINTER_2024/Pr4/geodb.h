#ifndef GEODB_H
#define GEODB_H

#include <string>
#include "geopoint.h"
#include "base_classes.h"
#include "HashMap.h"
#include <vector>

class GeoDatabase: public GeoDatabaseBase
{
public:
	GeoDatabase();
	virtual ~GeoDatabase();
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
	virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
	struct Data{
		std::string location_name;
		GeoPoint end_location;
	};
	HashMap<std::vector<Data>> m_connections; // The hashmap will contain for each G a geopoint a map from it's
	// G.to_string() method to a vector of Data Points with each 
	// Data point having a member GeoPoint E that is at the end of a line segment eminating from G
	// As well as a std::string object representing the name of the street of the line segment
	HashMap<GeoPoint> m_pois; // This second hashmap will relate each point of interest to one geoPoint
	HashMap<std::string> m_edge_names;
	std::string orderTwo(const GeoPoint& pt1, const GeoPoint& pt2) const;
};









#endif