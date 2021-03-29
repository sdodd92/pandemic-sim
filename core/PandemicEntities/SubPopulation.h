/*
 * SubPopulation.h
 *
 *  Created on: 2 Mar 2021
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_SUBPOPULATION_H_
#define CORE_PANDEMICENTITIES_SUBPOPULATION_H_

class SuperCommunity;

class SubPopulation {


public:
	SubPopulation();

	SubPopulation(SuperCommunity *base_pop);

	SubPopulation(SuperCommunity *base_pop, long n_subpops, int avg_pop_size);


	SubPopulation(SuperCommunity *base_pop, int avg_family_size);

	void mingle(int date, long *new_infected);
	void update(int date, long *dead, long *recovered);
	void set_lockdown() {locked_down=true;};
	void remove_lockdown() {locked_down=false;};

//	long mingle(int start_date, int end_date);


private:
	void *population, *subpops;
	long /* *subpop_sizes,*/ n_subpops;
	bool locked_down;
};

#endif /* CORE_PANDEMICENTITIES_SUBPOPULATION_H_ */
