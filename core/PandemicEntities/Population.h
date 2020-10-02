/*
 * Population.h
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_POPULATION_H_
#define CORE_PANDEMICENTITIES_POPULATION_H_

#include "Community.h"
#include "SubCommunity.h"
#include <vector>
#include <unordered_map>



class Population: public Community {
public:

	typedef std::unordered_map<unsigned long, unsigned long> NetworkNode;
	typedef std::unordered_map<unsigned long, NetworkNode> Network;

	typedef std::unordered_map<unsigned long, std::vector<unsigned long>> MembershipNode;

	Population();

	Population(long pop_size, double avg_compliance, double avg_resistance);

	void random_structure(long avg_community_size);
	void define_structure(long avg_community_size, unsigned long num_communities);

	unsigned long subpop_num_infected(long i) {return subcommunities[i].get_num_infected();};
	unsigned long* num_infected_breakdown();

	long* get_pop_sizes();
	long get_n_subcommunities() {return n_subcommunities;};

	long mingle(int date);

	Network get_person_network();
	Network get_community_network();
	MembershipNode get_community_memberships();


protected:
	std::vector<SubCommunity> subcommunities;
	long n_subcommunities;
};



#endif /* CORE_PANDEMICENTITIES_POPULATION_H_ */
