#ifndef ROUTER_H
#define ROUTER_H

#include "geodb.h"
#include "geotools.h"


class Router: public RouterBase
{
public:
	Router(const GeoDatabaseBase& geo_db);
	virtual ~Router();
	virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
	const GeoDatabaseBase& m_geo_db;
};





#endif