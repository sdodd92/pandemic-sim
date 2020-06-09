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
    mortality_rate = 0.0;
    incubation_period = 0;
    disease_length = 0;
    latent_period = 0;
    
    compute_kill_window();

    
}

Pathogen::Pathogen(double contagiousness, double mortality_rate, int disease_length, int incubation_period, int latent_period) : init_random() {
    this->contagiousness = contagiousness;
    this->incubation_period = incubation_period;
    this->disease_length = disease_length;
    this->latent_period = latent_period;
    this->mortality_rate = mortality_rate;
    
    compute_kill_window();
    
}

void Pathogen::compute_kill_window() {
    
    // derive chance of a kill each day from the overall mortality rate
    double non_mortality_rate = 1- mortality_rate;
    double disease_length_factor = 1.0 / (double)(disease_length);


    // draw number of days taken to kill this host 
    // todo incorporate host resistance
    double daily_mortality_rate = 1 - std::pow(non_mortality_rate, disease_length_factor);
    std::negative_binomial_distribution<int> draw_kill(1, daily_mortality_rate);
    kill_window = draw_kill(generator);
}

Pathogen::Pathogen(const Pathogen& orig)  : init_random() {
    // put some cool mutation logic here
    // (maybe eve link with evolutionary functionality...)
    contagiousness = orig.contagiousness;
    mortality_rate = orig.mortality_rate;
    incubation_period = orig.incubation_period;
    disease_length = orig.disease_length;
    latent_period = orig.latent_period;
    

    compute_kill_window();
    
}

Pathogen::~Pathogen() {
}

