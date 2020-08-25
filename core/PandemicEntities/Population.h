/*
 * Population.h
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_POPULATION_H_
#define CORE_PANDEMICENTITIES_POPULATION_H_

#include "Community.h"
#include <vector>

class Population: public Community {
public:
	Population();

	Population(long pop_size, double avg_compliance, double avg_resistance);

	void define_structure(long avg_community_size);

	unsigned long subpop_num_infected(long i) {return subcommunities[i].get_num_infected();};
	unsigned long* num_infected_breakdown();

	long* get_pop_sizes();
	long get_n_subcommunities() {return n_subcommunities;};

	long mingle(int date);



protected:
	std::vector<Community> subcommunities;
	long n_subcommunities;
};



#endif /* CORE_PANDEMICENTITIES_POPULATION_H_ */
