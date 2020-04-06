/* 
 * File:   Person.h
 * Author: sam
 *
 * Created on 03 April 2020, 14:30
 */

#ifndef PERSON_H
#define	PERSON_H

struct Pathogen {
    double 
        contagiousness, 
        mortality_rate
        ;
    int 
    disease_length,
    incubation_period,
    latent_period
    ;
} ;

class Person {
public:   
    Person();
    Person(const Person& orig);
    Person(double compliance, double resistance, double sociability);
    virtual ~Person();
    
    unsigned int get_date_infected() {
        int date_infected = this->infection_date;
        return date_infected;
    }
    
    bool is_infected() {
        bool infected = this->infected;
        return infected;
    }
    
    double get_sociability() {
        double sociability = this->sociability;
        return sociability;
    }
        
    bool catch_infection(Pathogen infection, int date, bool force);
    
    Pathogen pass_infection();
    
private:
    double compliance, resistance, sociability;
    
    Pathogen infection;
    
    bool
            infected=false,
            immune=false,
            sick=false,
            contagious=false,
            
            alive=true
    ;
    
    int infection_date;
    
    

};

#endif	/* PERSON_H */

