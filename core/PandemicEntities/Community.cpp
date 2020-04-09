/* 
 * File:   Community.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 16:45
 */

#include "Community.h"

Community::Community() {
    pop_size = 0;
}

Community::Community(const Community& orig) {
}

void Community::add_person(Person *person) {
    
    pop_size++;
    population.push_back(person);
    
}

void Community::add_person(double compliance, double resistance, double sociability) {
    Person *new_person = new Person(compliance, resistance, sociability);
    this->add_person(new_person);
}


unsigned long Community::initiate_infection(Pathogen pathogen, int date, unsigned long member) {
    
    Person *patient_zero = population[member];
    
    patient_zero->catch_infection(pathogen, date, true);
    unsigned long patient_zero_id = patient_zero->get_uid();
    
    return patient_zero_id;    
    
}

unsigned long Community::get_num_infected() {
    unsigned long num_infected = 0;
    
    for (Person *person : population) 
        if (person->is_infected())
            num_infected++;
    
    return num_infected;
}


long Community::pairwise_interaction(Person* person_1, Person* person_2, int date) {
    
    long is_infected = 0;
    
    if (person_1->is_infected()) {
        
        int date_delta = date - person_1->get_date_infected();
        Pathogen new_infection = person_1->pass_infection();
        
        int latent_period = new_infection.latent_period;
        int disease_course = new_infection.disease_length + new_infection.incubation_period;
        
        if (date_delta > latent_period & date_delta < disease_course) {
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            
            float infected = dist(generator);
            
            if (infected < new_infection.contagiousness) {
                //maybe add mutation logic here...?
                is_infected += person_2->catch_infection(new_infection, date, false);
            }
        }
        
    }
    
    return is_infected;
    
}

long Community::mingle(int date) {
    long new_infections = 0;
    long pop2 = 1;
    
    long popsize = this->pop_size;
    for (long i=0; i < popsize; i++) {
        
        Person *person_1 = (this->population)[i];
        int sociabiltiy = person_1->get_sociability(); // todo change this to pop-level
//        
//        std::poisson_distribution<int> distribution(sociabiltiy);
//        int interactions = distribution(this->generator);
        
        
        int interactions = sociabiltiy;
        for (int ii=0; ii < interactions; ii++) {
            std::uniform_int_distribution<long> pop_dist(0, popsize - 1);
            long j = pop_dist(this->generator);
            while (j == i && j < popsize)
                j = pop_dist(this->generator);
           
            Person *person_2 = (this->population)[j];
            
            new_infections += this->pairwise_interaction(person_1, person_2, 10);
        }
            
//    new_infections++;
    }
    
    
    return new_infections;
}

Community::~Community() {
}

