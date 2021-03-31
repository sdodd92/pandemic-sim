/*
 	entitymodule.h
 
   Created on: 1 Mar 2021
       Author: sam
 */

#ifndef CORE_RAW_ENTITIES_ENTITYMODULE_H_
#define CORE_RAW_ENTITIES_ENTITYMODULE_H_

#include <stdlib.h>

typedef void** Population;
typedef void** SubPops;
struct CommunityStructure {
	SubPops sub_communities;
	long n_sub_communities;
};


#ifdef __cplusplus
	extern "C" {
#endif

		// direct interfaces to the core subroutines (implemented in Fortran)
		void allocatePopulation(void** pop, long *pop_size);

		void minglePopulation(void **population, void **subpops, long *n_subpops, int *date, long *new_infected);
		void updatePopulation(void **population, int *date, long *new_dead, long *n_recovered);

		 void getNumDied(void **population, long *num_infected);

		void allocateSubPops(void **subpops, void **population, long *n_subpops, int *sociability);

		void initiateInfection(void **pop, double *contagious, double *mortality, int *length, int *latency, int *date, long *index);

		void allocateFamilies(void **families, void **pop, long *n_families, int *avg_family_size);
		
		long getCurrentInfected(void **population);

		void updateSpreaderFactor(void **subpops, float *new_spreader_factor, long *n_subpops);
		void updateSociability(void **subpops, float *new_sociability, long *n_subpops);

		void nullifyPopulation(void **population);


#ifdef __cplusplus
	}
#endif


inline Population newPopulation(long pop_size) {
	
	Population newPop = (Population)malloc(sizeof(void*));
	allocatePopulation(newPop, &pop_size);

	return newPop;
}

inline void freePopulation(Population population) {

	nullifyPopulation(population);
	free(population);
}


#endif /* CORE_RAW_ENTITIES_ENTITYMODULE_H_ */
