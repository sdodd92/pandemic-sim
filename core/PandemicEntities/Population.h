/*
 * Population.h
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_POPULATION_H_
#define CORE_PANDEMICENTITIES_POPULATION_H_

#include "Community.h"

class Population: public Community {
public:
	Population();

	Population(long pop_size, double clumpiness, double avg_compliance, double avg_resistance);

	virtual ~Population();


protected:
	Community* subcommunities;
};



#endif /* CORE_PANDEMICENTITIES_POPULATION_H_ */
