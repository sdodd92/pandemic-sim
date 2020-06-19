#' @export
plot.lockdownSim <- function(obj, option = "infection", add = FALSE) {

  result <- obj$epidemic_ts
  x <- result$day

  if (option == "infection") {
    y <- result$infected
  } else if (option == "death") {
    y <- result$cumu_death
  }

  plot(x, y, type = 'n')
  lines(x, y)
  abline(h = obj$infection_trigger, col = 'red', lty = 'dashed')


}

#' @title Simple Lockdown Run
#' @description run a simulation for a set number of days with a single set of lockdown parameters
#'
#' @param days the number of days to run for
#' @param population.params a list containing constructor parameters for the population
#' @param virus.params a list containing constructor parameters for the (initial) pathogen
#' @param sensitivity the level of infection at which lockdown will be triggered
#' @param aggressiveness the factor by which social interaction will be reduced when lockdown is applied
#'
#' @return an object of class "lockdownSim"
#'
#' @export
lockdownSim <- function(
  days,
  population.params,
  virus.params,
  lockdown.sensitivity,
  lockdown.aggressiveness
) {

  lockdown_result <- try_lockdown(
    days = days,
    popParams = population.params,
    virusParams = virus.params,
    sensitivity = lockdown.sensitivity,
    aggressiveness = lockdown.aggressiveness
  )

  result_ts <- data.frame(
    day = 1:days,
    infected = lockdown_result$infection_ts,
    cumu_death = lockdown_result$death_ts
  )


  return (
    structure(
      list(
        final_infected = lockdown_result$total_infected,
        final_dead = lockdown_result$total_dead,
        epidemic_ts = result_ts,
        infection_trigger = lockdown.sensitivity,
        attempted_reduction = lockdown.aggressiveness
      ),
      class = "lockdownSim"
    )
  )

}

#' @export
smartSim <- function(population.params, virus.params) {

  world <- generate_world(popParams = population.params, virusParams = virus.params)
  extra_params <- list(
    number_infected = c(),
    number_infections = c(),
    number_deaths = c(),
    date = c(),
    current_date = 0
  )

  return(structure(c(world, extra_params), class = c("smartSim", "lockdownSim")) )

}

#' @export
runSim <- function(sim, days = NULL, start_date = NULL, end_date = NULL) {

  if (is.null(start_date)) {
    start_date <- sim$current_date
  }

  if (is.null(end_date)) {
    if (is.null(days)) {
      warning("No end date provided. Simulation not run")
      return(sim)
    } else{
      end_date <- start_date + days
    }
  }

  new_sim <- increment_days(start_date, end_date, sim$raw_population)

  sim$number_infected <- c(sim$number_infected, new_sim$n_infected)
  sim$number_infections <- c(sim$number_infections, new_sim$n_infections)
  sim$number_deaths <- c(sim$number_deaths, new_sim$n_deaths)
  sim$date <- c(sim$date, start_date:end_date)

  return(sim)



}

