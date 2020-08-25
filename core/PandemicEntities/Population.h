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

	Population(long pop_size, double avg_compliance, double avg_resistance);

	void define_structure(double clumpiness);

	unsigned long subpop_num_infected(long i) {return subcommunities[i].get_num_infected();};
	unsigned long* num_infected_breakdown();

	long mingle(int date);


	virtual ~Population() {free(subcommunities);};


protected:
	Community* subcommunities;
	long n_subcommunities;
};



#endif /* CORE_PANDEMICENTITIES_POPULATION_H_ */
