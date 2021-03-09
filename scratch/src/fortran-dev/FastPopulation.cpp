	/*
	 * FastCommunity.cpp
	 *
	 *  Created on: 30 Oct 2020
	 *      Author: sam
	 */

	#include "FastPopulation.h"

	extern "C" {
	void defineFortranPop(void* pop, long *pop_size);

	void mingleFortranPop(void *population, void *subpops, long *n_subpops, int * date, long * new_infected);
	
	 void getFortranNumDied(void *population, long *num_infected);

	void defineFortranSubPops(void *subpops, void *population, long *n_subpops, int *sociability);

	void initFortranInfection(void *pop, double *contagious, double *mortality, int *length, int *latency, int *date, long *index);
	}

	FastCommunity::FastCommunity() {
		f_population = 0;
		f_subpops = 0;
	}

	FastCommunity::FastCommunity(long pop_size) {
		f_population = 0;
		f_subpops = 0;
		defineFortranPop(&f_population, &pop_size);
//		defineFortranPop(f_population, &pop_size);
	}

	void FastCommunity::define_subpops(long n_subpops, int sociability) {
		defineFortranSubPops(&f_subpops, &f_population, &n_subpops, &sociability);
		this->n_subpops = n_subpops;
	}

	long FastCommunity::mingle(int date) {
		long new_infected;
		mingleFortranPop(&f_population, &f_subpops, &n_subpops, &date, &new_infected);

		return new_infected;
	}

	void FastCommunity::initiate_infection(Pathogen &pathogen, int date, long index) {
		
		//convert from 0-indexing to 1-indexing
		date++;  
		index++;

		double contagiousness = pathogen.get_contagiousness();
		double mortality_rate = pathogen.get_mortality_rate();
		int disease_length = pathogen.get_disease_length();
		int latent_period = pathogen.get_latent_period();
		initFortranInfection(
			&f_population,
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






