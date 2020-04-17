/* 
 * File:   Pathogen.h
 * Author: sam
 *
 * Created on 17 April 2020, 14:17
 */

#ifndef PATHOGEN_H
#define	PATHOGEN_H

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
    
    double get_contagiousness() {
        return contagiousness;
    }
    
    double get_mortality_rate() {
        return mortality_rate;
    }
    double get_disease_length() {
        return disease_length;
    }  
    
    double get_incubation_period() {
        return incubation_period;
    }
    
    double get_latent_period() {
        return latent_period;
    }
    
    
    
private:
    double contagiousness, mortality_rate;
    
    int disease_length, incubation_period, latent_period;

};

#endif	/* PATHOGEN_H */

