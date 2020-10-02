#include <Rcpp.h>
#include "../../include/pandemicentities.h"
using namespace Rcpp;


void new_day(Community *pop, unsigned long* num_infected, unsigned long* total_infected, unsigned long* num_died, int date) {
  long new_infections = 0;

  (*total_infected) += pop->mingle(date);
  pop->update_health(date);

  *num_infected = pop->get_num_infected();
  *num_died  = pop->get_num_died();

}

// [[Rcpp::export]]
List test_params(int days, unsigned long pop_size, int sociability, double resistance, List virusParams) {

  Community *population = new Community(sociability);

  double compliance = 0;

  for (unsigned long p=0; p < pop_size; ++p)
    population->add_person(compliance, resistance);

  double contagiousness = (double)virusParams["contagiousness"];
  double mortality_rate = (double)virusParams["mortality_rate"];

  int incubation_period = (int)virusParams["incubation_period"];
  int disease_length = (int)virusParams["disease_length"];
  int latent_period = (int)virusParams["latent_period"];




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

  unsigned long total_infected = 0;


  IntegerVector cumu_infected(days);
  IntegerVector cumu_died(days);

  for (int d=0; d < days; ++d) {
    new_day(population, &num_infected, &total_infected, &num_died, d);
    cumu_infected(d) = (int)num_infected;
    cumu_died(d) = (int) num_died;
  }

  int total_dead = (int)population->get_num_died();

  return List::create(
    Named("total_infected") = total_infected,
    Named("infection_ts") = cumu_infected,
    Named("death_ts") = cumu_died,
    Named("total_dead") = total_dead
  );


}




// [[Rcpp::export]]
List try_lockdown(int days, List popParams, List virusParams, unsigned long sensitivity, double aggressiveness) {
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

  unsigned long total_infected = 0;


  IntegerVector cumu_infected(days);
  IntegerVector cumu_died(days);

  for (int d=0; d < days; ++d) {
    new_day(population, &num_infected, &total_infected, &num_died, d);
    cumu_infected(d) = (int)num_infected;
    cumu_died(d) = (int) num_died;
    if (num_infected >= sensitivity & population->get_sociability() == sociability) {
      // Rcout << "initiating lockdown on day " << d << "\n";
      population->reduce_sociability(aggressiveness);
    }

    else if (num_infected < sensitivity & population->get_sociability() != sociability) {
      // Rcout << "easing lockdown on day " << d << "\n";
      population->change_sociability(sociability);
    }

  }

  int total_dead = (int)population->get_num_died();

  return List::create(
    Named("total_infected") = total_infected,
    Named("infection_ts") = cumu_infected,
    Named("death_ts") = cumu_died,
    Named("total_dead") = total_dead
  );

}
