#include "tour_generator.h"
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router)
	: m_geodb(geodb), m_router(router) // get the basics in the constructor
{

}

TourGenerator::~TourGenerator(){} //no destructor necessary

std::string degreeToDirection(double dir){ //quickly convert degrees to directions
	if(0 <= dir && dir < 22.5){
		return "east";
	}
	else if(22.5 <= dir && dir < 67.5){
		return "northeast";
	}
	else if(67.5 <= dir && dir < 112.5){
		return "north";
	}
	else if(112.5 <= dir && dir < 157.5){
		return "northwest";
	}
	else if(157.5 <= dir && dir < 202.5){
		return "west";
	}
	else if(202.5 <= dir && dir < 247.5){
		return "southwest";
	}
	else if(247.5 <= dir && dir < 292.5){
		return "south";
	}
	else if(292.5 <= dir && dir < 337.5){
		return "southeast";
	}
	else if(dir >= 337.5){
		return "east";
	}
	return "error"; // a bad dir value was passed in
}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
	vector<TourCommand> result; // basic variables
	string poi;
	string talking_points;
	TourCommand t;
	int i;
	for(int i = 0; i < stops.size()-1; i++){ // iterate through the first n-1 stops // if there is only one stop there is no tour
		stops.get_poi_data(i, poi, talking_points); // start by talking about the first POI
		t.init_commentary(poi, talking_points);
		result.push_back(t);

		GeoPoint currentPoi, futurePoi; // now get to the next POI
		m_geodb.get_poi_location(poi, currentPoi);
		stops.get_poi_data(i+1, poi, talking_points);
		m_geodb.get_poi_location(poi, futurePoi);
		vector<GeoPoint> points = m_router.route(currentPoi, futurePoi); // start routing
		for(int j = 0; j < points.size()-1; j++) // go through the route that is taken
		{
			t.init_proceed(degreeToDirection(angle_of_line(points[j], points[j+1])), m_geodb.get_street_name(points[j]
				, points[j+1]), distance_earth_miles(points[j], points[j+1]), points[j], points[j+1]); // input the proceeding command
			result.push_back(t);
			if(j+2 < points.size()){//if there are 2 more points ahead
				if(m_geodb.get_street_name(points[j], points[j+1]) != m_geodb.get_street_name(points[j+1],
				 points[j+2])){//if we are getting onto a different street
				 	double turn_angle = angle_of_turn(points[j], points[j+1], points[j+2]); //tell the computer we are turning left or right
				 	if(1 <= turn_angle && turn_angle < 180){
				 		t.init_turn("left", m_geodb.get_street_name(points[j+1], points[j+2]));
				 		result.push_back(t); 
				 	}
				 	else if(180 <= turn_angle && turn_angle <= 359){
				 		t.init_turn("right", m_geodb.get_street_name(points[j+1], points[j+2]));
				 		result.push_back(t);
				 	}
				}
			}
		}


	}
	stops.get_poi_data(stops.size()-1, poi, talking_points); //finish the tour
	t.init_commentary(poi, talking_points);
	result.push_back(t);

	return result;
}
