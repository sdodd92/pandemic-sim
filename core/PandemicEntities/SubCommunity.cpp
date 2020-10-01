/*
 * SubCommunity.cpp
 *
 *  Created on: 30 Sep 2020
 *      Author: sam
 */

#include "SubCommunity.h"
#include "Person.h"
#include "Community.h"

#include <iterator>

SubCommunity::SubCommunity() : Community(-1) {}

void SubCommunity::add_person(Person* person) {
	Community::add_person(person);
	pop_uids.push_back(person->get_uid());
}

void SubCommunity::remove_person(unsigned long id) {
	pop_uids.erase(pop_uids.begin() + population[id]->get_uid());
	Community::remove_person(id);
}


unsigned long SubCommunity::shared_weight(const SubCommunity& other) {

	std::vector<unsigned long> intersection;

	std::set_intersection(
		this->pop_uids.begin(), this->pop_uids.end(),
		other.pop_uids.begin(), other.pop_uids.end(),
		std::back_inserter(intersection)
	);

	unsigned long intersect_size = intersection.size();
	return intersect_size;
}

