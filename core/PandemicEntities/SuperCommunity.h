/*
 * FastCommunity.h
 *
 *  Created on: 30 Oct 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_SUPERCOMMUNITY_H_
#define CORE_PANDEMICENTITIES_SUPERCOMMUNITY_H_

#include "Pathogen.h"
#include "SubPopulation.h"
#include <vector>


class SuperCommunity {
public:
	SuperCommunity();

	SuperCommunity(long pop_size);

	void define_subpops(long n_subpops, int sociability);

	long mingle(int date);

	long get_num_infected();

	void initiate_infection(Pathogen &pathogen, int date, long index=0);

	SubPopulation* define_generic_subpops(long n_subpops, int avg_pop_size);

	friend SubPopulation::SubPopulation(SuperCommunity *base_pop);
	friend SubPopulation::SubPopulation(SuperCommunity *base_pop, long n_subpops, int avg_pop_size);


protected:
	void *f_population, *f_subpops;
	long n_subpops;
	std::vector<SubPopulation> community_layers;
};

#endif /* CORE_PANDEMICENTITIES_SUPERCOMMUNITY_H_ */