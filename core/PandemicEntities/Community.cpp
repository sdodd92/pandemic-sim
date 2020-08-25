/* 
 * File:   Community.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 16:45
 */

#include "Community.h"
#include <stdlib.h>

#define init_random() generator(std::random_device{}())

Community::Community() : init_random() {
    pop_size = 0;
    base_sociability = 0;
    population = (Person**)malloc(sizeof(Person*));
}

Community::Community(int sociability) : init_random() {
    pop_size = 0;
    base_sociability = sociability;
    population = (Person**)malloc(sizeof(Person*));
}

Community::Community(int sociability, long pop_size, double avg_compliance, double avg_resistance) : init_random() {
	this->pop_size = 0; // initialize pop-size as zero so as not to break the add-person loop below (this is messy :( )
	base_sociability = sociability;

	for (int i=0; i<pop_size; ++i)
		add_person(avg_compliance, avg_resistance); //todo draw compliance/resistance from distribution

}



bool Community::add_person(Person *person) {
    
    for (int i=0; i < pop_size; ++i)
    	// reject the addition of this person is already in the population
    	if (population[i]->get_uid() == person->get_uid())
    		return false;
    if (pop_size == 0){
    	population[0] = person;
    } else {
    	population = (Person**)realloc(population, (pop_size + 1)*sizeof(Person*));
    	population[pop_size] = person;
    }
    pop_size++;
    

    return true;

}

bool Community::add_person(double compliance, double resistance) {
    Person *new_person = new Person(compliance, resistance);
    bool success = add_person(new_person);

    return success;
}


unsigned long Community::initiate_infection(Pathogen *pathogen, int date, unsigned long member) {
    
    Person *patient_zero = population[member];
    
    patient_zero->catch_infection(pathogen, date, true);
    unsigned long patient_zero_id = patient_zero->get_uid();
    
    return patient_zero_id;    
    
}

unsigned long Community::get_num_infected() {
    unsigned long num_infected = 0;
    
    Person* person;
    for (int i=0; i < pop_size; ++i) {
    	person = population[i];
        if (person->is_alive() & person->is_infected())
            num_infected++;
    }
    
    return num_infected;
}


long Community::pairwise_interaction(Person* person_1, Person* person_2, int date) {
    
    long is_infected = 0;
    std::uniform_real_distribution<float> draw_infected_prob(0.0, 1.0);
    
    if (person_1->is_infected()) {
        
        int date_delta = date - person_1->get_date_infected();
        Pathogen *new_infection = person_1->pass_infection();
        
        int latent_period = new_infection->get_latent_period();
        int disease_course = new_infection->get_disease_length() + new_infection->get_incubation_period();
        
        if (date_delta > latent_period & date_delta < disease_course) {
            

            float infected_roll = draw_infected_prob(generator);
            
            if (infected_roll < new_infection->get_contagiousness()) {
                //maybe add mutation logic here...?
                is_infected += person_2->catch_infection(new_infection, date, false);
            }
        }
        
    }
    
    return is_infected;
    
}

long Community::mingle(int date) {
    long new_infections = 0; 
    
    Person *person_1, *person_2;
    

    std::uniform_int_distribution<long> draw_interactee(0, pop_size - 1);


    for (long i=0;pop_size > 1 & i < pop_size; i++) {
        
        person_1 = population[i];
        
        if (person_1->is_alive()) { //don't iterate on dead ppl (efficiency...)
            
            int sociability;
            if (base_sociability < 0) {
            // negative sociability factor means all possible interactions occur
                for (long j=0; j < pop_size; ++j) 
                    if (j != i) {
                        person_2 = population[j];
                        new_infections += pairwise_interaction(person_1, person_2, date);
                    };
                        
            } else {
            // otherwise draw number of interactions from poisson mean
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
                new_infections += pairwise_interaction(person_1, person_2, date);
                new_infections += pairwise_interaction(person_2, person_1, date);

            } // for (int ii=0; ii < interactions; ii++)
            
           } // if (base_sociability < 0)
        } // if (person_1->is_alive())
    } // long Community::mingle(int date)
    
    
    return new_infections;
}

unsigned long Community::update_health(int date) {
    unsigned long new_deaths = 0;
    
    Person* person;
    for (int i=0; i < pop_size; ++i) {
    	person = population[i];
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

    Person* person;
    for (int i=0; i < pop_size; ++i) {
    	person = population[i];
        if (!person->is_alive())
            num_died++;
    }
    
    return num_died;
}



void Community::reduce_sociability(double reduction_factor) {
    
    if (base_sociability >= 0)
        base_sociability *= reduction_factor;
    else
        base_sociability = pop_size * reduction_factor;
    
    
}

