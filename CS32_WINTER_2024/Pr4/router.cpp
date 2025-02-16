
#include "router.h"
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db) : m_geo_db(geo_db)
{}

Router::~Router()
{}

std::vector<GeoPoint> reconstruct_path(unordered_map<string, GeoPoint> cameFrom, GeoPoint& current)
{
	std::stack<GeoPoint> total_path; //creates a stack for every location visited in reverse order
	total_path.push(current); // get the ending location
	while(cameFrom.find(current.to_string()) != cameFrom.end()){ // while there are elements in stack
		current = cameFrom[current.to_string()]; // get the element that came previous and push it
		total_path.push(current);
	}

	std::vector<GeoPoint> v; // now to construct vector
	while(!total_path.empty()) {
		v.push_back(total_path.top()); // just get every element from stack and put in vector
		total_path.pop();
	}
	return v;
}


std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	unordered_map<string, GeoPoint> cameFrom; // hashmap recording progress
	unordered_map<string, double> gScore; // gScore is the total amount of distance traveled to get to a point
	unordered_map<string, double> fScore; // fScore is the total amount of distance traveled to get to point + distance to end (underestimate)

	auto h = [&](const GeoPoint& pta, const GeoPoint& ptb) {
	            return (fScore[pta.to_string()]) > (fScore[ptb.to_string()]); // lambda function used to sort heap elements in the priority queue
	};

	priority_queue<GeoPoint, vector<GeoPoint>, decltype(h)> openSet(h); // openset is a priority_queue to get the element with the lowest fScore at any time
	unordered_set<string> inopenSet; //keep track of everything that has been addded to the openSet
	gScore[pt1.to_string()] = 0; // set the things
	fScore[pt1.to_string()] = distance_earth_km(pt1, pt2); // initial fScore
	openSet.push(pt1); // get the starting element into the queuue - should have done this after setting fScore but it doesnt matter because it is the first element
	inopenSet.insert(pt1.to_string()); // 





	while(!openSet.empty()){ // start A*
		GeoPoint current = openSet.top(); // get curr element
		if(current.sLatitude == pt2.sLatitude && current.sLongitude == pt2.sLongitude){ //check to see if finished
			return reconstruct_path(cameFrom, current);
		}
		openSet.pop();
		vector<GeoPoint> connections = m_geo_db.get_connected_points(current); // iterate through every connection
		for(int i = 0; i < connections.size(); i++){
			auto current_gScore = gScore.find(current.to_string()); // get the current gscore to update the future gScore
			double tentative_gScore = current_gScore->second + distance_earth_km(current, connections[i]); //what would the gScore be if I used this point

			unordered_map<string,double>::const_iterator neighbor_g = gScore.find(connections[i].to_string()); //what is the actual gScore
			if(neighbor_g != gScore.end()){ // we have been here before
				if(tentative_gScore >= neighbor_g->second){ // if this current connection is slower than the previous fastest way to get here then discard it
					continue;
				}
			}
			cameFrom[connections[i].to_string()] = current; // assuming this is the fastest way we have made it to this point before
			gScore[connections[i].to_string()] = tentative_gScore; // update the gScore
			fScore[connections[i].to_string()] = tentative_gScore + distance_earth_km(connections[i], pt2); //update the fscore
			if(inopenSet.find(connections[i].to_string()) == inopenSet.end()){ //if this point is not in the open set yet
				openSet.push(connections[i]);//add the point to the openSet
				inopenSet.insert(connections[i].to_string()); // record that it is in the openSet
			}
		}
	}
	cerr << "no path found" << endl; // error message that there was no route found
	return std::vector<GeoPoint>();
}

