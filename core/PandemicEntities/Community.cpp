/* 
 * File:   Community.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 16:45
 */

#include "Community.h"

Community::Community() {
    pop_size = 0;
    base_sociability = 0;
}

Community::Community(int sociability) {
    pop_size = 0;
    base_sociability = sociability;
}


void Community::add_person(Person *person) {
    
    pop_size++;
    population.push_back(person);
    
}

void Community::add_person(double compliance, double resistance) {
    Person *new_person = new Person(compliance, resistance);
    this->add_person(new_person);
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


long Community::pairwise_interaction(Person* person_1, Person* person_2, int date) {
    
    long is_infected = 0;
    
    if (person_1->is_infected()) {
        
        int date_delta = date - person_1->get_date_infected();
        Pathogen *new_infection = person_1->pass_infection();
        
        int latent_period = new_infection->get_latent_period();
        int disease_course = new_infection->get_disease_length() + new_infection->get_incubation_period();
        
        if (date_delta > latent_period & date_delta < disease_course) {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            
            float infected = dist(generator);
            
            if (infected < new_infection->get_contagiousness()) {
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
    
    #ifdef PARALLEL_MINGLE
    #pragma omp parallel for private(person_1, person_2)
    #endif
	// TODO debug systematic differences from parallelization
    for (long i=0;i < pop_size; i++) {
        
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
                std::uniform_int_distribution<long> draw_interactee(0, pop_size - 1);
                
                long j = draw_interactee(generator);
                person_2 = population[j];
                
                while (i == j | !person_2->is_alive()){
                    // re-draw if the interactee is the same
                    // or if the interactee is dead
                    j = draw_interactee(generator);
                    person_2 = population[j];
                }


                new_infections += pairwise_interaction(person_1, person_2, date);
            } // for (int ii=0; ii < interactions; ii++)
            
           } // if (base_sociability < 0)
        } // if (person_1->is_alive())
    } // long Community::mingle(int date)
    
    
    return new_infections;
}

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

