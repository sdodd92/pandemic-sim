library(Rpandemic)
library(doParallel)
library(foreach)
library(scales)
library(viridis)
library(ggplot2)

registerDoParallel(3)

pop.size <- 1e6

population.params <- list(
  sociability = 20,
  pop_size = pop.size,
  compliance = 0.8,
  resistance = .1
)

virus.params <- list(
  contagiousness = 0.02,
  mortality_rate = 0.05,
  incubation_period = 5,
  disease_length = 14,
  latent_period = 2
)

days <- 90
lockdown.aggressiveness <- 0.3

triggers <- pop.size * c(.01, .05, .1, .2, 1)

lockdown_results <- foreach(
  trigger = triggers,
  .combine = "rbind",
  .multicombine = TRUE,
  .packages = "Rpandemic"
) %dopar% {
  
  result <- lockown_run(
    days = days,
    population.params = population.params,
    virus.params = virus.params,
    lockdown.sensitivity = trigger,
    lockdown.aggressiveness = lockdown.aggressiveness
  )
  
  run_outcome <- result$epidemic_ts
  run_outcome$lockdown_trigger <- trigger
  run_outcome$final_infections <- result$final_infected
  
  return(run_outcome)
  
  
}

ggplot(lockdown_results, aes(x=day, y=infected, color=as.factor(lockdown_trigger / pop.size))) +
  scale_y_continuous(labels = comma) +
  scale_color_viridis(discrete = TRUE) +
  geom_line()

lockdown_results$infected.pct <- lockdown_results$infected / pop.size

ggplot(lockdown_results, aes(x=day, y=infected.pct, color=as.factor(lockdown_trigger / pop.size))) +
  scale_y_continuous(labels = percent) +
  scale_color_viridis(discrete = TRUE) +
  geom_line()

ggplot(lockdown_results, aes(x=day, y=cumu_death, color=as.factor(lockdown_trigger / pop.size))) +
  scale_y_continuous(labels = comma) +
  scale_color_viridis(discrete = TRUE) +
  geom_line()

ggplot(lockdown_results, aes(x=as.factor(lockdown_trigger / pop.size), y=final_infections)) +
  stat_summary(fun.y = "max", geom = "bar")
