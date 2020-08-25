/*
 * Population.cpp
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#include "Population.h"
#include "Border.h"
#include <stdlib.h>
#include <random>
#include <unordered_set>

Population::Population() : Community() {
	// initialize a single starting sub-community
	subcommunities = (Community*)malloc(sizeof(Community));
	n_subcommunities = 1;
}

Population::Population(long pop_size, double clumpiness, double avg_compliance, double avg_resistance)
 : Community(0, pop_size, avg_compliance, avg_resistance) {

	// initialize a single starting sub-community
	subcommunities = (Community*)malloc(sizeof(Community));
	n_subcommunities = 1;

}


void Population::define_structure(double clumpiness) {

	std::uniform_int_distribution<long> draw_member_index(0, pop_size - 1);
	std::unordered_set<long> clumped_ids;

	long avg_community_size = (long)(pop_size * clumpiness);

	// initialize count of un-allocated individuals
	long un_clumped = pop_size;
	long n_clumped = 0;

	while (clumped_ids.size() < pop_size) {
			std::poisson_distribution<long> draw_community_size(avg_community_size);
			long community_size = draw_community_size(generator);

			// expand the subcommunity array after filling the initial member
			if (n_subcommunities > 1)
				subcommunities = (Community*)realloc(subcommunities, (n_subcommunities + 1) * sizeof(Community));

			// initialize the new sub-community and increment the running counter
			subcommunities[n_subcommunities] = Community(-1);  // negative sociability means all possible interactions
			n_subcommunities++;

			// get a pointer to the new community (readability)
			Community* new_community = &(subcommunities[n_subcommunities]);

			for (long n=0; n < community_size; ++n) {

				bool success = false;
				long i;
				while (!success) {
					i = draw_member_index(generator);
					success = Border::share(this, new_community, i);
				} // while !success

				clumped_ids.insert(i);

			} // for (long n=0; n < community_size; ++n)


		} // while (clumped_ids.size() < pop_size)

} // Population::define_structure(double clumpiness)


long Population::mingle(int date) {

	/*
	 * for a super-community, mingling is defined as each of its sub-populations mixing with each other
	 *
	 * note mingling within sub-populations is NOT independent as members can also be members of other sub-populations
	 * programmatically, the entry of the virus into a new sub-community is dependent on members' histories
	 */
	long new_infections = 0;
	for (int i=1; i < n_subcommunities; ++i)
		new_infections += subcommunities[i].mingle(date);

	return new_infections;

}

unsigned long* Population::num_infected_breakdown() {

	unsigned long* output = (unsigned long*)malloc(n_subcommunities * sizeof(unsigned long));
	for (int i=0; i < n_subcommunities; ++i) {
		output[i] = subcommunities[i].get_num_infected();
	}

	return output;

}

