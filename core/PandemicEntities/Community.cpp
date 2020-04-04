/* 
 * File:   Community.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 16:45
 */

#include "Community.h"

Community::Community() {
    this->pop_size = 0;
}

Community::Community(const Community& orig) {
}

void Community::add_person(Person person) {
    
    this->pop_size++;
    this->population.push_back(person);
    
}

long Community::pairwise_interaction(Person* person_1, Person* person_2, int date) {
    
    long is_infected = 0;
    
    if (person_1->is_infected()) {
        
        int date_delta = person_1->get_date_infected() - date;
        Pathogen new_infection = person_1->pass_infection();
        
        int latent_period = new_infection.latent_period;
        int disease_course = new_infection.disease_length + new_infection.incubation_period;
        
        if (date_delta > latent_period & date_delta < disease_course) {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            
            float infected = dist(this->generator);
            
            if (infected < new_infection.contagiousness) {
                //maybe add mutation logic here...?
                person_2->catch_infection(new_infection, date);
                is_infected = 1;
            }
        }
        
    }
    
    return is_infected;
    
}

long Community::mingle(int date) {
    
//    std::default_random_engine generator;
    
    long new_infections = 0;
    
    for (long i; i < this->pop_size; i++) {
        
        Person *person_1 = &(this->population)[i];
        int sociabiltiy = person_1->get_sociability();
        
        std::poisson_distribution<int> distribution(sociabiltiy);
        
        int interactions = distribution(this->generator);
        
        for (unsigned int interaction=0; interaction < interactions; interaction++) {
            unsigned int j;
            int max_pop = this->pop_size - 1;
            std::uniform_int_distribution<> dist2(0, max_pop);
            j = dist2(this->generator);
            while (j == i)
                j = dist2(this->generator);
            Person *person_2 = &(this->population)[j];
            new_infections += this->pairwise_interaction(person_1, person_2, date);
                        
        }
        
    }
    
    return new_infections;
    
}

Community::~Community() {
}

