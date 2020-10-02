/*
 * Border.cpp
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#include "Border.h"



bool Border::share(Community* pop_a, Community* pop_b, unsigned long member_index) {
	bool success = pop_b->safely_add_person(pop_a->get_person(member_index));
	return success;
}

void Border::exchange(unsigned long member_index) {
	share(pop_a, pop_b, member_index);
	pop_a->remove_person(member_index);
}
