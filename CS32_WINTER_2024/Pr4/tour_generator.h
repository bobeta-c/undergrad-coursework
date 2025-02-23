#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H

#include "geodb.h"
#include "router.h"
#include <vector>
#include "tourcmd.h"


class TourGenerator: public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
	virtual ~TourGenerator();
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
	const GeoDatabaseBase& m_geodb;
	const RouterBase& m_router;
};





#endif
