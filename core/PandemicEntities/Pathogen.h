/* 
 * File:   Pathogen.h
 * Author: sam
 *
 * Created on 17 April 2020, 14:17
 */

#ifndef PATHOGEN_H
#define	PATHOGEN_H

#include <random>
#include <ctime>



class Pathogen {
public:
    Pathogen();
    
    Pathogen(
            double contagiousness, 
            double mortality_rate,
            int disease_length,
            int incubation_period,
            int latent_period
    );
    
    Pathogen(const Pathogen& orig);
    virtual ~Pathogen();
    
    bool kill(double host_resistance);
    bool kill() {return true;}
    
    double get_contagiousness() {
        return contagiousness;
    }
    
    double get_mortality_rate() {
        return mortality_rate;
    }
    int get_disease_length() {
        return disease_length;
    }  
    
    int get_incubation_period() {
        return incubation_period;
    }
    
    int get_latent_period() {
        return latent_period;
    }
    
    
    
private:
    double contagiousness, mortality_rate;
    
    int disease_length, incubation_period, latent_period;
    
    

    std::mt19937 generator;
    

};

#endif	/* PATHOGEN_H */

