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
	subcommunities = (Community*)malloc(sizeof(Community));
}

Population::Population(long pop_size, double clumpiness, double avg_compliance, double avg_resistance)
 : Community() {

	std::uniform_int_distribution<long> draw_member_index(0, pop_size - 1);
	std::unordered_set<long> clumped_ids;

	long avg_community_size = (long)(pop_size * clumpiness);

	// initialize count of un-allocated individuals
	long un_clumped = pop_size;
	long n_clumped = 0;

	subcommunities = (Community*)malloc(sizeof(Community));
	long n_communities = 1;

	while (clumped_ids.size() < pop_size) {
		std::poisson_distribution<long> draw_community_size(avg_community_size);
		long community_size = draw_community_size(generator);

		// expand the subcommunity array after filling the initial member
		if (n_communities > 1)
			subcommunities = (Community*)realloc(subcommunities, (n_communities + 1) * sizeof(Community));

		// initialize the new sub-community and increment the running counter
		subcommunities[n_communities] = Community(-1);
		n_communities++;

		// get a pointer to the new community (readability)
		Community* new_community = &(subcommunities[n_communities]);

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


}


Population::~Population() {
	// TODO Auto-generated destructor stub
}

