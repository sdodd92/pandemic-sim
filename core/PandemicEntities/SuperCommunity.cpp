	/*
	 * FastCommunity.cpp
	 *
	 *  Created on: 30 Oct 2020
	 *      Author: sam
	 */

#include "SuperCommunity.h"
#include "../raw-entities/entitymodule.h"

#include "SubPopulation.h"

SuperCommunity::SuperCommunity() {
	pop = 0;
	f_subpops = 0;
	n_subpops = 0;
}

SuperCommunity::SuperCommunity(long pop_size) {
	f_subpops = 0;

	n_subpops = 0;
	pop = newPopulation(pop_size);
//		defineFortranPop(f_population, &pop_size);
}

void SuperCommunity::define_subpops(long n_subpops, int sociability) {
	allocateSubPops(&f_subpops, pop, &n_subpops, &sociability);
	this->n_subpops = n_subpops;
}

void SuperCommunity::mingle(int date, long &new_infected, long &new_dead, long &recovered) {
	
	for (auto community : community_layers) {
		community.mingle(date, &new_infected);
		community.update(date, &new_dead, &recovered);
	}
}

void SuperCommunity::initiate_infection(Pathogen &pathogen, int date, long index) {

	//convert from 0-indexing to 1-indexing

	double contagiousness = pathogen.get_contagiousness();
	double mortality_rate = pathogen.get_mortality_rate();
	int disease_length = pathogen.get_disease_length();
	int latent_period = pathogen.get_latent_period();
	initiateInfection(
		pop,
		&contagiousness,
		&mortality_rate,
		&disease_length,
		&latent_period,
		&date,
		&index

	);

}

//	long FastCommunity::get_num_infected() {
//		long num_infected;
//
//		getFortranNumDied(f_population, &num_infected);
//
//		return num_infected;
//
//	}


SubPopulation* SuperCommunity::define_generic_subpops(long n_subpops, int avg_pop_size) {
	community_layers.push_back(SubPopulation(this, n_subpops, avg_pop_size));

	return &community_layers.back();
}

SubPopulation* SuperCommunity::define_families(int avg_family_size) {
	community_layers.push_back(SubPopulation(this, avg_family_size));

	return &community_layers.back();
}


