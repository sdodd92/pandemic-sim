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

#' @export
lockown_run <- function(
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

