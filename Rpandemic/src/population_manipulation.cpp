#include <Rcpp.h>
#include "../../core/pandemicentities.h"
#include "pandemicInterface.h"
using namespace Rcpp;


// [[Rcpp::export]]
List generate_world(List popParams, List virusParams) {

  int sociability = popParams["sociability"];
  Community *population = new Community(sociability);

  for (unsigned long p=0; p < (unsigned long)popParams["pop_size"]; ++p)
    population->add_person((double)popParams["compliance"], (double)popParams["resistance"]);

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
  XPtr<Community> raw_population(population);

  PROTECT(raw_virus);
  PROTECT(raw_population);

  return List::create(
    Named("raw_virus") = raw_virus,
    Named("raw_population") = raw_population,
    Named("pop_size") = popParams["pop_size"]
  );

}

// [[Rcpp::export]]
List increment_days(int start_date, int end_date, SEXP population) {

  // unpack the Community instance which was passed back from R
  Community* pop;

  unpack_ptr<Community>(population, &pop);


  int num_days = end_date - start_date;

  // declare time-series stat trackers
  NumericVector newly_infected(num_days);
  NumericVector current_infected(num_days);
  NumericVector number_died(num_days);


    long new_infections = 0;
    int current_date = start_date;

    for (int d=0; d < num_days; ++d) {

      newly_infected(d) = pop->mingle(current_date);
      current_infected(d) = pop->get_num_infected();
      pop->update_health(current_date);
      number_died(d) = pop->get_num_died();

      ++current_date;

    }

    return List::create(
      Named("raw_population") = population,
      Named("n_infections") = newly_infected,
      Named("n_infected") = current_infected,
      Named("n_deaths") = number_died
    );


}

// [[Rcpp::export]]
SEXP change_lockdown(SEXP population, int new_sociability) {

  Community* pop;

  unpack_ptr<Community>(population, &pop);

  pop->change_sociability(new_sociability);

  return population;

}



