#include <pandemicentities.h>
#include <iostream>
#include <omp.h>
int main() {

	const unsigned long POP_SIZE = 1000000;
	const double COMPLIANCE = 0.5;
	const double RESISTANCE = 0.5;

	const int SOCIABILITY = 20;
	const long N_COMMUNITIES = 200000;

	const int N_DAYS = 30;

	SuperCommunity mypop(POP_SIZE);
	
//	mypop.define_subpops(N_COMMUNITIES, SOCIABILITY);
	SubPopulation *sub_pop = mypop.define_generic_subpops(N_COMMUNITIES, SOCIABILITY);

	Pathogen virus(.5, .02, 31, 0, 2);

	mypop.initiate_infection(virus, 0, 0);

//	mypop.initiate_infection(virus, 0, 10);
	#ifndef NOPAR
	omp_set_num_threads(8);
	#endif

	unsigned long num_infected[N_DAYS];
//	unsigned long num_died[N_DAYS];

	num_infected[0] = 0;
//	num_died[0] = 0;

	#pragma omp parallel
	{
	for (int i=1; i<N_DAYS; ++i)
		num_infected[i] = mypop.mingle(i);
	}

//		mypop.update_health(d);
//		num_infected[d] = mypop.get_num_infected();
//		num_died[d] = mypop.get_num_died();
//	}

	std::cout << "DATE,INFECTED,DIED\n";
	for (int i=0; i<N_DAYS; ++i)
		std::cout << i << ',' << num_infected[i]/* << ',' << num_died[i]*/ << '\n';

	



	return 0;
}	
