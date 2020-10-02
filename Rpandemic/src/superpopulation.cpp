#include <Rcpp.h>
#include "../../include/pandemicentities.h"
#include "pandemicInterface.h"
#include <string>


using namespace Rcpp;


// [[Rcpp::export]]
List generate_population(List popParams, List virusParams) {
  
  int sociability = popParams["sociability"];
  Population *population = new Population((unsigned long)popParams["pop_size"], (double)popParams["compliance"], (double)popParams["resistance"]);
  
  population->define_structure((int)popParams["sociability"], 2000);

  double contagiousness = virusParams["contagiousness"];
  double mortality_rate = virusParams["mortality_rate"];
  
  int incubation_period = virusParams["incubation_period"];
  int disease_length = virusParams["disease_length"];
  int latent_period = virusParams["latent_period"];
  
  
  
  
  Pathogen *virus = new Pathogen(
    contagiousness,
    mortality_rate,
    disease_length,
    incubation_period,
    latent_period
  );
  
  int date = 0;
  
  population->initiate_infection(virus, date, 0); // arbitrarily pick the first person as patient zero
  
  unsigned long num_infected = 1; // we know one person is infected cause we forced it
  unsigned long num_died = 0;
  
  // wrap up the relevant instances to pass back to R
  XPtr<Pathogen> raw_virus(virus);
  XPtr<Population> raw_population(population);
  
  PROTECT(raw_virus);
  PROTECT(raw_population);
  
  return List::create(
    Named("raw_virus") = raw_virus,
    Named("raw_population") = raw_population,
    Named("pop_size") = popParams["pop_size"]
  );
  
}

// [[Rcpp::export]]
List subpop_memberships(SEXP population) {
  Population* pop;
  
  unpack_ptr<Population>(population, &pop);
  
  // unsigned long *infection_numbers = pop->num_infected_breakdown();
  Population::MembershipNode memberships = pop->get_community_memberships();
  
  List pop_list(memberships.size());
  
  
  for (auto& membership : memberships)
    pop_list[membership.first] = membership.second;
  
  return pop_list;
  
  
}

// [[Rcpp::export]]
List subpop_connections(SEXP population) {
  Population* pop;
  
  unpack_ptr<Population>(population, &pop);
  
  Population::Network connections = pop->get_community_network();
  
  List pop_network;
  
  for (auto& pop_from : connections) {
    List network_entry;
    // Rcout << "From: " << pop_from.first << ": " << pop_from.second.size()<< '\n';
    for (auto& pop_to : pop_from.second) {
      // Rcout << "Making entry for " << pop_to.first << '\n';
      std::string to_key = std::to_string(pop_to.first);
      network_entry[to_key] = pop_to.second;
      // network_entry[pop_to.first] = pop_to.second;
    }
    std::string from_key = std::to_string(pop_from.first);
    pop_network[from_key] = network_entry;
    // pop_network[pop_from.first] = network_entry;
  }
  
  return pop_network;
  
  
}

// [[Rcpp::export]]
NumericVector subpop_infections(SEXP population) {
  
  Population* pop;
  unpack_ptr<Population>(population, &pop);
  
  unsigned long *raw_infection_numbers = pop->num_infected_breakdown();
  
  long n_subpops = pop->get_n_subcommunities();
  
  NumericVector infection_numbers(n_subpops);
  for (long i=0; i<n_subpops; ++i)
    infection_numbers(i) = raw_infection_numbers[i];
  
  return infection_numbers;
  
}