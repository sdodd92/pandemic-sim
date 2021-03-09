#include <pandemicentities.h>
#include <iostream>
//#include <stdlib.h>
#include <omp.h>
int main() {

	const unsigned long POP_SIZE = 1000000;
	const double COMPLIANCE = 0.5;
	const double RESISTANCE = 0.5;

	const int SOCIABILITY = 20;
	const long N_COMMUNITIES = 200000;

	const int N_DAYS = 30;

	Population mypop(POP_SIZE, COMPLIANCE, RESISTANCE);
	
	mypop.define_structure(SOCIABILITY, N_COMMUNITIES);

	Pathogen *virus = new Pathogen(.5, 0.02, 31, 0, 2);

	mypop.initiate_infection(virus, 0, 0);
	omp_set_num_threads(8);

	unsigned long num_infected[N_DAYS];
	unsigned long num_died[N_DAYS];

	num_infected[0] = 0;
	num_died[0] = 0;

	for (int d=1; d<N_DAYS; ++d) {
		mypop.mingle(d);

		mypop.update_health(d);
		num_infected[d] = mypop.get_num_infected();
		num_died[d] = mypop.get_num_died();
	}

//	long *community_sizes = mypop.get_pop_sizes();

//	std::cout << "ID,SIZE\n";
//	for (int i=0; i<N_COMMUNITIES; ++i)
//		std::cout << i << ',' << community_sizes[i] << '\n';
//	free(community_sizes);
	std::cout << "DATE,INFECTED,DIED\n";
	for (int i=0; i<N_DAYS; ++i)
		std::cout << i << ',' << num_infected[i] << ',' << num_died[i] << '\n';


	delete virus;
	



	return 0;
}	
