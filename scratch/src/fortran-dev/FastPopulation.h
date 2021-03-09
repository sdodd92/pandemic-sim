/*
 * FastCommunity.h
 *
 *  Created on: 30 Oct 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_FASTCOMMUNITY_H_
#define CORE_PANDEMICENTITIES_FASTCOMMUNITY_H_

#include "../../../include/pandemicentities.h"

class FastCommunity {
public:
	FastCommunity();

	FastCommunity(long pop_size);

	void define_subpops(long n_subpops, int sociability);

	long mingle(int date);

	long get_num_infected();

	void initiate_infection(Pathogen &pathogen, int date, long index=0);


protected:
	void *f_population, *f_subpops;
	long n_subpops;
};

#endif /* CORE_PANDEMICENTITIES_FASTCOMMUNITY_H_ */

