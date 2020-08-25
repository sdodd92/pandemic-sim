/* 
 * File:   Community.h
 * Author: sam
 *
 * Created on 03 April 2020, 16:45
 */

#ifndef COMMUNITY_H
#define	COMMUNITY_H

#include "Person.h"
#include <random>
#include <vector>
#include <iostream>
#include <string>
#include <omp.h>

using namespace std;

class Community {
public:
    Community();
//    Community(const Community& orig);
    
    Community(int sociability);
    
    Community(int sociability, long pop_size, double avg_compliance, double avg_resistance);

    void add_person(Person *person);
    void add_person(double compliance, double resistance);
    bool safely_add_person(Person *person);
    void remove_person(unsigned long id);
    
    long get_pop_size() {return pop_size;};
    Person* get_person(long index) {return population[index];};

    long mingle(int date);
    
    unsigned long initiate_infection(Pathogen *pathogen, int date, unsigned long member);
    
    unsigned long get_num_infected();
    
    unsigned long update_health(int date);
    
    unsigned long get_num_died();
    
    int get_sociability() {return base_sociability;};
    
    void change_sociability(int new_sociability) 
    {base_sociability = new_sociability;} ;
    
    void reduce_sociability(double reduction_factor);
    
protected:
	
    long pop_size;
    std::vector<Person*> population;
    
    int base_sociability;

    
    std::mt19937 generator;
    
    long pairwise_interaction(Person *person_1, Person *person_2, int date);
};

#endif	/* COMMUNITY_H */

