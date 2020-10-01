/*
 * SubCommunity.cpp
 *
 *  Created on: 30 Sep 2020
 *      Author: sam
 */

#include "SubCommunity.h"
#include "Person.h"
#include "Community.h"

SubCommunity::SubCommunity() : Community(-1) {}

void SubCommunity::add_person(Person* person) {
	Community::add_person(person);
	pop_uids.insert(person->get_uid());
}

void SubCommunity::remove_person(unsigned long id) {
	pop_uids.erase(population[id]->get_uid());
	Community::remove_person(id);
}


