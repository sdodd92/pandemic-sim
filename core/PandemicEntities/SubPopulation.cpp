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
	this->population = base_pop->pop;
	subpops=0;
	n_subpops=0;
	locked_down=false;
//	subpop_sizes=0;
}

SubPopulation::SubPopulation(SuperCommunity* base_pop, long n_subpops, int avg_pop_size) {
	allocateSubPops(&subpops, base_pop->pop, &n_subpops, &avg_pop_size);
	this->population = base_pop->pop;
	this->n_subpops = n_subpops;
	locked_down=false;
}

SubPopulation::SubPopulation(SuperCommunity* base_pop, int avg_family_size) {
	allocateFamilies(&subpops, base_pop->pop, &n_subpops, &avg_family_size);
	this->population = base_pop->pop;
	locked_down=false;
}

void SubPopulation::mingle(int date, long *new_infected) {

	// if the population is fully shut down, it cannot mingle
	if (locked_down) {
		*new_infected = 0;
		return;
	}

	minglePopulation(population, &subpops, &n_subpops, &date, new_infected);

}

void SubPopulation::update(int date, long *new_dead, long *recovered) {
	updatePopulation(population, &date, new_dead, recovered);

}
