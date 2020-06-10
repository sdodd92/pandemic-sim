/* 
 * File:   Pathogen.cpp
 * Author: sam
 * 
 * Created on 17 April 2020, 14:17
 */

#include "Pathogen.h"
#include <random>
#include <cmath>

#define init_random() generator(std::random_device{}())
    

Pathogen::Pathogen() : init_random() {
    // default to something that doesn't actually cause a disease
    contagiousness = 0.0;
    incubation_period = 0;
    disease_length = 0;
    latent_period = 0;
    daily_mortality_rate = 0.0;

    
}

Pathogen::Pathogen(double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period) : init_random() {
    this->contagiousness = contagiousness;
    this->incubation_period = incubation_period;
    this->disease_length = disease_length;
    this->latent_period = latent_period;
    
    // derive chance of a kill each day from the overall mortality rate
    double non_mortality_rate = 1- mortality_rate;
    double disease_length_factor = 1.0 / (double)(disease_length);
    
    daily_mortality_rate = 1 - std::pow(non_mortality_rate, disease_length_factor);
    
}

Pathogen::Pathogen(const Pathogen& orig)  : init_random() {
    // put some cool mutation logic here
    // (maybe eve link with evolutionary functionality...)
    contagiousness = orig.contagiousness;
    daily_mortality_rate = orig.daily_mortality_rate;
    incubation_period = orig.incubation_period;
    disease_length = orig.disease_length;
    latent_period = orig.latent_period;
}

double Pathogen::get_mortality_rate() {
    
    double non_mortality_rate = std::pow(1 - daily_mortality_rate, disease_length);
    double overall_mortality_rate = 1 - non_mortality_rate;
    
    return overall_mortality_rate;
    
}


bool Pathogen::attempt_kill(double host_resistance) {
    bool kill = false;
    
    // reduce the daily chance of a kill by the host's personal resistance
    double mortality_chance = daily_mortality_rate * (1- host_resistance);
    std::uniform_real_distribution<double> draw_kill;
    double kill_roll = draw_kill(generator);
    if (kill_roll < mortality_chance)
        kill = true;
    return kill;
}

Pathogen::~Pathogen() {
}

