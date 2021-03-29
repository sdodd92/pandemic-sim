/*
 	entitymodule.h
 
   Created on: 1 Mar 2021
       Author: sam
 */

#ifndef CORE_RAW_ENTITIES_ENTITYMODULE_H_
#define CORE_RAW_ENTITIES_ENTITYMODULE_H_


#ifdef __cplusplus
	extern "C" {
#endif

		// direct interfaces to the core subroutines (implemented in Fortran)
		void defineFortranPop(void* pop, long *pop_size);

		void mingleFortranPop(void *population, void *subpops, long *n_subpops, int *date, long *new_infected);
		void updatePopulation(void *population, int *date, long *new_dead, long *n_recovered);

		 void getFortranNumDied(void *population, long *num_infected);

		void defineFortranSubPops(void *subpops, void *population, long *n_subpops, int *sociability);

		void initFortranInfection(void *pop, double *contagious, double *mortality, int *length, int *latency, int *date, long *index);

		void defineFortranFamilies(void *families, void *pop, long *n_families, int *avg_family_size);
		
		long getCurrentInfected(void *population);

		void updateSpreaderFactor(void *subpops, float *new_spreader_factor);


#ifdef __cplusplus
	}
#endif


#endif /* CORE_RAW_ENTITIES_ENTITYMODULE_H_ */
