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
plot.smartSim <- function(sim, what = "total_infected", date.origin = NULL) {

  X <- sim$date
  if (!is.null(date.origin)) {
    X <- as.Date(X, origin = date.origin)
  }

  if (what == "total_infected") {
    Y<- sim$number_infected
  } else if (what == "new_infections") {
    Y <- sim$number_infections
  } else if (what == "deaths") {
    Y <- sim$number_deaths
  }

  plot(X, Y, type = 'n')
  lines(X, Y)

}

#' @export
smartSim <- function(population.params, virus.params) {

  world <- generate_world(popParams = population.params, virusParams = virus.params)
  extra_params <- list(
    number_infected = c(),
    number_infections = c(),
    number_deaths = c(),
    date = c(),
    current_date = 0,
    current_sociability = population.params$sociability,
    base.sociability = population.params$sociability
  )

  return(structure(c(world, extra_params), class = "smartSim") )

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
  sim$date <- c(sim$date, (start_date + 1):end_date)
  sim$current_date <- end_date

  return(sim)

}

#' @export
update <- function(sim, lockdown_factor = NULL, new.sociability = NULL) {

  if (!is.null(lockdown_factor)) {
    new.sociability <- as.integer(sim$base.sociability * lockdown_factor)
  }

  sim$raw_population <- change_lockdown(sim$raw_population, new.sociability)
  sim$current_sociability <- new.sociability

  return(sim)

}

