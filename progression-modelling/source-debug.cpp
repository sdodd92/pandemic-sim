#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
void debugger1(double contagiousness, double mortality_rate, int incubation_period, int disease_length, int latent_period) {
  
  Rcout << "mortality rate from raw param is " << mortality_rate << "\n";
  
}


void debugger2(List params) {
  
  double mortality_rate = (double)params["mortality_rate"];
  Rcout << "mortality rate from list is " << mortality_rate << "\n";
}

/*** R

params <- list(
  contagiousness = 0.2,
  mortality_rate = 0.02,
  incubation_period = 1,
  disease_length = 12,
  latent_period = 2
)

with(params, debugger1(contagiousness, mortality_rate, incubation_period, disease_length, latent_period))
debugger2(params)

*/
