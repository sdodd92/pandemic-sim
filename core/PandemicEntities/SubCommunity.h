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
#include <vector>
#include <algorithm>


class SubCommunity: public Community {
public:
	SubCommunity();

	void add_person(Person* person) override;
	void remove_person(unsigned long id) override;

	void sort_uids() {std::sort(pop_uids.begin(), pop_uids.end());};

	unsigned long shared_weight(const SubCommunity& other);

protected:
	std::vector<unsigned long> pop_uids;
};

#endif /* CORE_PANDEMICENTITIES_SUBCOMMUNITY_H_ */
