/*
 * Border.h
 *
 *  Created on: 20 Aug 2020
 *      Author: sam
 */

#ifndef CORE_BORDER_H_
#define CORE_BORDER_H_

#include "Community.h"

class Border {
public:
	Border() {
		pop_a = 0;
		pop_b = 0;

	};

	Border(Community* pop_a, Community* pop_b) {
		this->pop_a = pop_a;
		this->pop_b = pop_b;
	};

	static bool share(Community* pop_a, Community* pop_b);
	static bool share(Community* pop_a, Community* pop_b, unsigned long member_index);

//	void exchange(); //currently not implemented
	void exchange(unsigned long member_index);

protected:
	Community* pop_a;
	Community* pop_b;
};

#endif /* CORE_BORDER_H_ */
