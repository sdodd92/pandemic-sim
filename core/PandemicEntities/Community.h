/* 
 * File:   Community.h
 * Author: sam
 *
 * Created on 03 April 2020, 16:45
 */

#ifndef COMMUNITY_H
#define	COMMUNITY_H

#include "Person.h"
#include <vector>
#include <random>
#include <iostream>
#include <string>

using namespace std;

class Community {
public:
    Community();
//    Community(const Community& orig);
    
    Community(int sociability);
    
    void add_person(Person *person);
    void add_person(double compliance, double resistance);
    void remove_person(unsigned long id);
    
    long mingle(int date);
    
    unsigned long initiate_infection(Pathogen *pathogen, int date, unsigned long member);
    
    unsigned long get_num_infected();
    
    unsigned long update_health(int date);
    
    unsigned long get_num_died();
    
    int get_sociability() {return base_sociability;};
    
    void change_sociability(int new_sociability) 
    {base_sociability = new_sociability;} ;
    
    void reduce_sociability(double reduction_factor);
    
//    virtual ~Community();
protected:
    long pop_size;
    vector<Person*> population;
    
    int base_sociability;

    
    std::random_device r;
    std::mt19937 generator{r()};
    
    long pairwise_interaction(Person *person_1, Person *person_2, int date);
};

#endif	/* COMMUNITY_H */

