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

using namespace std;

class Community {
public:
    Community();
    Community(const Community& orig);
    
    void add_person(Person *person);
    void add_person(double compliance, double resistance, double sociability);
    void remove_person(unsigned long id);
    
    long mingle(int date);
    
    unsigned long initiate_infection(Pathogen pathogen, int date, unsigned long member);
    
    unsigned long get_num_infected();
    
    virtual ~Community();
private:
    long pop_size;
    vector<Person*> population;
    
    std::default_random_engine generator;
    
    long pairwise_interaction(Person *person_1, Person *person_2, int date);
};

#endif	/* COMMUNITY_H */

