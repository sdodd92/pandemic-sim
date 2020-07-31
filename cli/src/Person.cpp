/* 
 * File:   Person.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 14:30
 */

#include "Person.h"
#include "Pathogen.h"


unsigned long Person::uid_counter = 0;

void Person::increment_uid() {
    uid_counter++;
}

Person::Person() {
    resistance = 0.5;
    compliance = 0.5;
    
    increment_uid();
}

Person::Person(double compliance_factor, double resistance_factor) {
    
    resistance = resistance_factor;
    compliance = compliance_factor;
   
    uid = uid_counter;
    increment_uid();
    
}

unsigned long Person::get_uid() {
    unsigned long this_uid = uid;
    return this_uid;
}


bool Person::catch_infection(Pathogen *infection, int date, bool force) {
    bool new_infected = false;
    if (force | !(infected | immune)) {
        infected = true;
        immune = true;
        this->infection = infection;
        infection_date = date;
        
        new_infected = true;
        
    }
    
    
//    Pathogen new_infection = this->infection;
    
    return new_infected;
}

bool Person::survival_update(int date) {
    bool survived = true;
    if (infected) {
        
        int days_elapsed = date - infection_date ;      
        int incubation_period = infection->get_incubation_period();
        int total_length = infection->get_disease_length() + incubation_period;
        if (days_elapsed >= total_length) {
            sick = false;
            infected = false;
        }
        else if (days_elapsed > incubation_period) {
            sick = true;
            if (infection->attempt_kill(resistance)) {
                alive = false;
                survived = false;
            }
        }        
        return survived;
        
    }
    
}


Pathogen* Person::pass_infection() {
    
    Pathogen *new_infection = infection;
    
    // put more complex code here...?
    
    return new_infection;
    
}

Person::~Person() {
}

