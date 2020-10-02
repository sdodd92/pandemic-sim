/* 
 * File:   Person.h
 * Author: sam
 *
 * Created on 03 April 2020, 14:30
 */

#ifndef PERSON_H
#define	PERSON_H

#include "Pathogen.h"

class Person {
public:   
    Person();
    Person(const Person& orig);
    Person(double compliance, double resistance);
    
    unsigned int get_date_infected() {
        int date_infected = this->infection_date;
        return date_infected;
    }
    
    bool is_infected() {
        bool infected = this->infected;
        return infected;
    }
    
    
    unsigned long get_uid();
        
    bool catch_infection(Pathogen *infection, int date, bool force);
    
    bool survival_update(int date);
    
    bool is_alive() {return alive;};
    bool is_contagious() {return contagious;};
    
    bool infect(int date, Person* new_host);
    
    Pathogen* get_infection() {return infection;};

private:
    double compliance, resistance;
    
    static void increment_uid();
    
    Pathogen *infection;
    
    bool
            infected=false,
            immune=false,
            sick=false,
            contagious=false,
            
            alive=true
    ;
    
    int infection_date;
    
    unsigned long uid;
    static unsigned long uid_counter;
    
    

};

#endif	/* PERSON_H */

