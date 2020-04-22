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
        
        int days_elapsed  ;      
//        int incubation_period = infection->get_incubation_period();
        if ((days_elapsed = date - infection_date) < infection->get_disease_length()) {
            sick = true;
            if (infection->kill(resistance)) {
                alive = false;
                survived = false;
            }
                
        } else {
            sick = false;
            infected = false;
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

