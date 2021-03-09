/* 
 * File:   Community.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 16:45
 */

#include "Community.h"

#define init_random() generator(std::random_device{}())

Community::Community() : init_random() {
    pop_size = 0;
    base_sociability = 0;
}

Community::Community(int sociability) : init_random() {
    pop_size = 0;
    base_sociability = sociability;
}

Community::Community(int sociability, long pop_size, double avg_compliance, double avg_resistance) : init_random() {
	this->pop_size = 0; // initialize pop-size as zero so as not to break the add-person loop below (this is messy :( )
	base_sociability = sociability;

	for (int i=0; i<pop_size; ++i)
		add_person(avg_compliance, avg_resistance); //todo draw compliance/resistance from distribution

}



void Community::add_person(Person *person) {
    
    population.push_back(person);
    pop_size++;

}

bool Community::safely_add_person(Person* person) {
    for (int i=0; i < pop_size; ++i)
    	// reject the addition of this person is already in the population
    	if (population[i]->get_uid() == person->get_uid())
    		return false;
    
    add_person(person);
    return true;
}

void Community::add_person(double compliance, double resistance) {
    Person *new_person = new Person(compliance, resistance);
    add_person(new_person);

}


unsigned long Community::initiate_infection(Pathogen *pathogen, int date, unsigned long member) {
    
    Person *patient_zero = population[member];
    
    patient_zero->catch_infection(pathogen, date, true);
    unsigned long patient_zero_id = patient_zero->get_uid();
    
    return patient_zero_id;    
    
}

unsigned long Community::get_num_infected() {
    unsigned long num_infected = 0;
    
    for (Person *person : population)
        if (person->is_alive() & person->is_infected())
            num_infected++;
    
    return num_infected;
}


long Community::mingle(int date) {
    long new_infections = 0; 
    
    if (base_sociability > 0) {
    	new_infections += mingle_partial(date);
    } else if (base_sociability < 0) {
    	new_infections += mingle_all(date);
    }
    // if base_sociability = 0 then don't do anything

    return new_infections;
}

long Community::mingle_all(int date) {

	/*
	 * this is a more efficient interaction algorithm which can be applied if all members are guaranteed to interact
	 *
	 * in this case, we simply iterate through the population until an infection is found
	 *
	 * if an infection is found, we iterate through the population again to expose everyone
	 *
	 * interaction runtime should in this case scale roughly linearly with population size
	 */

	long new_infections;

	Pathogen* virus = NULL; // todo convert this to a set to accomodate mutations
	int date_delta;

	for (Person* person_1 : population) {
		if (person_1->is_contagious()) {
			virus = person_1->get_infection();
			date_delta = date - person_1->get_date_infected();
			break;
		}
	}

	// exit if no infection was found
	if (virus == NULL) return 0;

	//otherwise, expose each vulnerable person to the virus
	for (Person* person_2 : population)
		new_infections += virus->infect(person_2, date_delta, date);


	return new_infections;

}

long Community::mingle_partial(int date) {

	/*
	 * this is probably the most costly function in the entire model
	 * ...
	 */

	long new_infections = 0;

    Person *person_1, *person_2;
    

    std::uniform_int_distribution<long> draw_interactee(0, pop_size - 1);

	#pragma omp for
    for (long i=0;i < pop_size; i++) {
        
        person_1 = population[i];
        
        if (person_1->is_alive()) { //don't iterate on dead ppl (efficiency...)
            
            int sociability;
            //draw number of interactions from poisson mean
            sociability = base_sociability;

            std::poisson_distribution<int> draw_interaction_count(sociability);
            int interactions = draw_interaction_count(generator);

            for (int ii=0; ii < interactions; ii++) { 

                // select n interactees based on the count drawn above
                long j = draw_interactee(generator);
                person_2 = population[j];
                
                while (i == j | !person_2->is_alive()){
                    // re-draw if the interactee is the same
                    // or if the interactee is dead
                    j = draw_interactee(generator);
                    person_2 = population[j];
                }

                // todo put logging logic here

                // interaction should be symmetrical (each person has equal chance of infecting the other)
                new_infections += person_1->infect(date, person_2);

            } // for (int ii=0; ii < interactions; ii++)
            
        } // if (person_1->is_alive())

    } // for (long i=0;pop_size > 1 & i < pop_size; i++)

    return new_infections;
} // long Community::mingle(int date)


unsigned long Community::update_health(int date) {
    unsigned long new_deaths = 0;
    
    for (Person *person : population) {
        if (person->is_alive())
            if (!person->survival_update(date))
                new_deaths++;
    }
//        if (person->is_alive())
//            if (!person->survival_update(date))
//                new_deaths++;
    
    return new_deaths;
}

unsigned long Community::get_num_died() {
    unsigned long num_died = 0;

    for (Person *person : population)
        if (!person->is_alive())
            num_died++;
    
    return num_died;
}



void Community::reduce_sociability(double reduction_factor) {
    
    if (base_sociability >= 0)
        base_sociability *= reduction_factor;
    else
        base_sociability = pop_size * reduction_factor;
    
    
}

void Community::remove_person(unsigned long id) {

	population.erase(population.begin() + id);
	pop_size--;

}


void Community::update_infection_log(int date) {

	for (Person* person : population)
		infection_log[date][person->get_uid()] = person->is_infected();

}


