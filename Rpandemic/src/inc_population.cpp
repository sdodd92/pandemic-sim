#include <Rcpp.h>
#include "../../core/pandemicentities.h"
using namespace Rcpp;


void new_day(Community *pop, unsigned long* num_infected, unsigned long* num_died, int date) {
  long new_infections = 0;

  new_infections += pop->mingle(date);

  *num_infected += new_infections;
  *num_died += pop->update_health(date);

}

// [[Rcpp::export]]
List test_params(int days, unsigned long pop_size, int sociability, double contagiousness, double mortality_rate, double resistance, int disease_length, int incubation_period, int latent_period) {

  Community *population = new Community(sociability);

  double compliance = 0.5;

  for (unsigned long p=0; p < pop_size; ++p)
    population->add_person(compliance, resistance);



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


  IntegerVector cumu_infected(days);
  IntegerVector cumu_died(days);

  for (int d=0; d < days; ++d) {
    new_day(population, &num_infected, &num_died, d);
    cumu_infected(d) = (int)num_infected;
    cumu_died(d) = (int) num_died;
  }

  int total_infected = (int)population->get_num_infected(); //intentionally use a different source to check for agreement
  int total_dead = (int)population->get_num_died();

  return List::create(
    Named("total_infected") = total_infected,
    Named("infection_ts") = cumu_infected,
    Named("death_ts") = cumu_died,
    Named("total_dead") = total_dead
  );


}
