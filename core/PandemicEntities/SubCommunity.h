/*
 * SubCommunity.h
 *
 *  Created on: 30 Sep 2020
 *      Author: sam
 */

#ifndef CORE_PANDEMICENTITIES_SUBCOMMUNITY_H_
#define CORE_PANDEMICENTITIES_SUBCOMMUNITY_H_

#include "Community.h"
#include "Person.h"
#include <unordered_set>


class SubCommunity: public Community {
public:
	SubCommunity();

	void add_person(Person* person) override;
	void remove_person(unsigned long id) override;

	bool population_contains(unsigned long id) {
		auto finder = pop_uids.find(id);
		if (finder != pop_uids.end())
			return false;
		else return true;
	};

protected:
	std::unordered_set<unsigned long> pop_uids;
};

#endif /* CORE_PANDEMICENTITIES_SUBCOMMUNITY_H_ */
