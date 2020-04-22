/* 
 * File:   Pathogen.cpp
 * Author: sam
 * 
 * Created on 17 April 2020, 14:17
 */

#include "Pathogen.h"

Pathogen::Pathogen() {
    // default to something that doesn't actually cause a disease
    contagiousness = 0.0;
    mortality_rate = 0.0;
    incubation_period = 0.0;
    disease_length = 0.0;
    latent_period = 0.0;
    
    
//    srand(time (NULL ));
}

Pathogen::Pathogen(double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period) {
    this->contagiousness = contagiousness;
    this->mortality_rate = mortality_rate;
    this->incubation_period = incubation_period;
    this->disease_length = disease_length;
    this->latent_period = latent_period;
    
//    srand(time (NULL ));
}

Pathogen::Pathogen(const Pathogen& orig) {
    // put some cool mutation logic here
    // (maybe eve link with evolutionary functionality...)
    contagiousness = orig.contagiousness;
    mortality_rate = orig.mortality_rate;
    incubation_period = orig.incubation_period;
    disease_length = orig.disease_length;
    latent_period = orig.latent_period;
}

bool Pathogen::kill(double host_resistance) {
    bool kill = false;
    double mortality_chance = mortality_rate / (double)disease_length;
    mortality_chance *= 1 - host_resistance;
    std::uniform_real_distribution<double> draw_kill;
    double kill_roll = draw_kill(generator);
    if (kill_roll < mortality_chance)
        kill = true;
    return kill;
}

Pathogen::~Pathogen() {
}
