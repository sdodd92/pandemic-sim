/*
 * SubPopulation.cpp
 *
 *  Created on: 2 Mar 2021
 *      Author: sam
 */

#include "SubPopulation.h"
#include "../raw-entities/entitymodule.h"
#include "SuperCommunity.h"

SubPopulation::SubPopulation() {
	population=0;
	subpops=0;
	n_subpops=0;
	locked_down=false;
//	subpop_sizes=0;

}

SubPopulation::SubPopulation(SuperCommunity* base_pop) {
	this->population = base_pop->f_population;
	subpops=0;
	n_subpops=0;
	locked_down=false;
//	subpop_sizes=0;
}

SubPopulation::SubPopulation(SuperCommunity* base_pop, long n_subpops, int avg_pop_size) {
	subpops=0;
	defineFortranSubPops(&subpops, &(base_pop->f_population), &n_subpops, &avg_pop_size);
	this->population = base_pop->f_population;
	this->n_subpops = n_subpops;
	locked_down=false;
}

long SubPopulation::mingle(int date) {
	long new_infected;

	if (locked_down)
		return 0;

	mingleFortranPop(&population, &subpops, &n_subpops, &date, &new_infected);

	return new_infected;
}
