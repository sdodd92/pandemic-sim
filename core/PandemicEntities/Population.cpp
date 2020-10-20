/*
 * Population.cpp
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#include "SubCommunity.h"
#include "Population.h"
#include "Border.h"
#include <stdlib.h>
#include <random>
#include <unordered_set>

Population::Population() : Community() {
	// initialize a single starting sub-community
	n_subcommunities = 0;
}

Population::Population(long pop_size, double avg_compliance, double avg_resistance)
 : Community(0, pop_size, avg_compliance, avg_resistance) {

	// initialize a single starting sub-community
	n_subcommunities = 0;

}


void Population::random_structure(long avg_community_size) {

	std::uniform_int_distribution<long> draw_member_index(0, pop_size - 1);
	std::unordered_set<long> clumped_ids;
	std::poisson_distribution<long> draw_community_size(avg_community_size);

	// initialize count of un-allocated individuals
	long un_clumped = pop_size;
	long n_clumped = 0;

	while (clumped_ids.size() < pop_size) {
			long community_size = draw_community_size(generator);

			// initialize the new sub-community and increment the running counter
			subcommunities.push_back(SubCommunity()); // negative sociability means all possible interactions
			n_subcommunities++;

			// get a pointer to the new community (readability)
			Community* new_community = &(subcommunities[n_subcommunities - 1]);

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

} // Population::define_structure(long avg_community_size)


void Population::allocate_structure(long num_communities, std::uniform_int_distribution<long> &draw_member_index, std::poisson_distribution<long> &draw_community_size) {
#pragma omp parallel
	{

		std::vector<SubCommunity> tmp_subcommunities;
		Community* new_community;
		int i;

		#pragma omp for nowait
		for (i=0; i < num_communities; ++i) {

			long community_size = draw_community_size(generator);

			// initialize the new sub-community and increment the running counter
			tmp_subcommunities.push_back(SubCommunity()); // negative sociability means all possible interactions

			// get a pointer to the new community (readability)
			new_community = &tmp_subcommunities.back();

			for (long n=0; n < community_size; ++n) {

				bool success = false;
				long i;
				while (!success) {
					i = draw_member_index(generator);
					success = Border::share(this, new_community, i);
				} // while !success

			} // for (long n=0; n < community_size; ++n)


		} // for (int i=0; i < num_communities; ++i)

		#pragma omp critical
		subcommunities.insert(subcommunities.end(), tmp_subcommunities.begin(), tmp_subcommunities.end());


	} // #pragma omp parallel

	n_subcommunities += num_communities;
}




void Population::define_structure(long avg_community_size, long num_communities, long from_index,long to_index) {

	if (to_index == NULL) to_index = pop_size - 1;

	std::uniform_int_distribution<long> draw_member_index(from_index, to_index);
	std::poisson_distribution<long> draw_community_size(avg_community_size);

	allocate_structure(num_communities, draw_member_index, draw_community_size);
}


void Population::define_tight_structure(int avg_community_size) {

	std::poisson_distribution<int> draw_community_size(avg_community_size);

	long pop_start = 0, pop_end = 0;
	while (pop_start < pop_size) {

		int pop_size = draw_community_size(generator);
		pop_end = pop_start + pop_size;
		if (pop_end > pop_size) pop_end = pop_size;
		subcommunities.push_back(SubCommunity());

		for (long i = pop_start; i < pop_end; ++i)
			Border::share(this, &subcommunities.back(), i);

		pop_start = pop_end + 1;

	}
}

long Population::mingle(int date) {

	/*
	 * for a super-community, mingling is defined as each of its sub-populations mixing with each other
	 *
	 * note mingling within sub-populations is NOT independent as members can also be members of other sub-populations
	 * programmatically, the entry of the virus into a new sub-community is dependent on members' histories
	 */
	long new_infections = 0;
	for (Community& subcommunity : subcommunities)
		new_infections += subcommunity.mingle(date);

	return new_infections;

}

unsigned long* Population::num_infected_breakdown() {

	unsigned long* output = (unsigned long*)malloc(n_subcommunities * sizeof(unsigned long));
	for (int i=0; i < n_subcommunities; ++i) {
		output[i] = subcommunities[i].get_num_infected();
	}

	return output;

}


long* Population::get_pop_sizes() {

	long* output = (long*)malloc(n_subcommunities * sizeof(long));
	for (int i=0; i < n_subcommunities; ++i) {
		output[i] = subcommunities[i].get_pop_size();
	}

	return output;

}

Population::Network Population::get_person_network() {

	/*
	 * this function returns a nested map representing an undirected network of person-to-person relationships
	 * a single entry is recorded for all unordered pairs of persons which co-occur at least once in a subpopulation
	 * the inner value of this nested map represents the weight of the relationship
	 * the weight corresponds to the number of sub-communities in which this pair exists
	 */

	Network network;
	unsigned long node_from, node_to, pop_size;

	// iterate through each subcommunity to find pairwise relationships
	for (const Community& community : subcommunities) {

		pop_size = community.get_pop_size();

		// record each unordered pair of population members
		for (unsigned long i=0; i < pop_size; ++i)
			for (unsigned long j = (i + 1); j < pop_size; ++j) {
				node_from = community.get_person(i)->get_uid();
				node_to = community.get_person(j)->get_uid();
				network[node_from][node_to]++;  // integer edge weight corresponds to the number of communities in which the members co-occur
			}
	}


	return network;

}

Population::Network Population::get_community_network() {
	Network network;

	unsigned long link_count;

	for (SubCommunity& subcommunity : subcommunities)
		subcommunity.sort_uids();

	for (long i=0; i<n_subcommunities; ++i)
		for (long j=(i + 1); j<n_subcommunities; ++j) {
			link_count = subcommunities[i].shared_weight(subcommunities[j]);
			if (link_count > 0)
				network[i][j] += link_count;
		}

	return network;
}

Population::MembershipNode Population::get_community_memberships() {

	MembershipNode memberships;

	#pragma omp parallel
	{
		MembershipNode tmp_memberships;
		Community* subcommunity;

		#pragma omp for nowait
		for (unsigned long i=0; i < n_subcommunities; ++i) {
			subcommunity = &(subcommunities[i]);
			for (unsigned long j=0; j < subcommunity->get_pop_size(); ++j)
				tmp_memberships[i].push_back(subcommunity->get_person(j)->get_uid());
		}

		#pragma omp critical
		memberships.insert(tmp_memberships.begin(), tmp_memberships.end());

	}

	return memberships;

}




















