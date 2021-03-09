/*
 * entities_module.h
 *
 *  Created on: 1 Mar 2021
 *      Author: sam
 */

#ifndef CORE_RAW_ENTITIES_ENTITYMODULE_H_
#define CORE_RAW_ENTITIES_ENTITYMODULE_H_


#ifdef __cplusplus
	extern "C" {
#endif

		void defineFortranPop(void* pop, long *pop_size);

		void mingleFortranPop(void *population, void *subpops, long *n_subpops, int *date, long *new_infected);

		 void getFortranNumDied(void *population, long *num_infected);

		void defineFortranSubPops(void *subpops, void *population, long *n_subpops, int *sociability);

		void initFortranInfection(void *pop, double *contagious, double *mortality, int *length, int *latency, int *date, long *index);

#ifdef __cplusplus
	}
#endif

#endif /* CORE_RAW_ENTITIES_ENTITYMODULE_H_ */
