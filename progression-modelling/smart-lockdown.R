library(Rpandemic)
library(magrittr)

update_interval <- 21
total_run_time <- 360

population.params <- list(
  sociability = 20,
  pop_size = 1e6,
  resistance = .5,
  compliance = 1.
)

virus.params <- list(
  contagiousness = .02,
  mortality_rate = .05,
  incubation_period =7,
  latent_period = 3,
  disease_length = 14
)


lockdown_sensitivity <- population.params$pop_size * 2e-5  # the threshold at which the UK actually went into lockdown
# lockdown_sensitivity <- population.params$pop_size * .1
lockdown_degree <- .25

date <- 1
lockdown_dates <- c()
release_dates <- c()

start.time <- Sys.time()

world <- smartSim(
  population.params = population.params,
  virus.params = virus.params
)

while (date <= total_run_time) {
  
  world %<>% runSim(days = update_interval)
  
  if (world$number_infections[date] >= lockdown_sensitivity & 
      world$current_sociability == world$base.sociability) {
    world %<>% update(lockdown_factor = lockdown_degree)
    lockdown_dates <- c(lockdown_dates, date + update_interval)
  } else if (world$number_infections[date] < lockdown_sensitivity & 
             world$current_sociability < world$base.sociability) {
    world %<>% update(new.sociability = world$base.sociability)
    release_dates <- c(release_dates, (date + update_interval))
  }
 
  date <- date + update_interval 
}

end.time <- Sys.time()
print(end.time - start.time)

cumu_infected <- cumsum(world$number_infections)



# plot(world, what = "new_infections", date.origin = "2020-01-22")
plot(world, what = "total_infected", date.origin = "2020-01-22")
abline(v = as.Date(lockdown_dates, origin = "2020-01-22"), col = "red")
abline(v = as.Date(release_dates, origin = "2020-01-22"), col = "green")
abline(h = lockdown_sensitivity, col = "orange")
