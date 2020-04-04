/* 
 * File:   Person.cpp
 * Author: sam
 * 
 * Created on 03 April 2020, 14:30
 */

#include "Person.h"

Person::Person() {
}

Person::Person(double compliance, double resistance, double sociability) {
    
    this->resistance = resistance;
    this->compliance = compliance;
    this->sociability = sociability;
       
}

Pathogen Person::catch_infection(Pathogen infection, int date) {
    if (!(this->infected | this->immune)) {
        this->infected = true;
        this->immune = true;
        this->infection = infection;
        this->infection_date = date;
    }
    
    
    Pathogen new_infection = this->infection;
    
    return new_infection;
}


Pathogen Person::pass_infection() {
    
    Pathogen new_infection = this->infection;
    
    // put more complex code here...?
    
    return new_infection;
    
}
Person::Person(const Person& orig) {
}

Person::~Person() {
}

