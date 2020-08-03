library(Rpandemic)
library(doParallel)
library(foreach)
library(scales)
library(viridis)
library(ggplot2)

registerDoParallel(4)

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

days <- 360

severities <- c(.75, .5, .3, .25, .15, .1)
triggers <- pop.size * c(.05, .1, .2)

# severities <- c(.75, .5)
# triggers <- pop.size * c(.1, .2)


lockdown_results <- foreach(
  severity = severities,
  .combine = "rbind",
  .multicombine = TRUE,
  .packages = "Rpandemic"
) %:% foreach(
  trigger = triggers,
  .combine = "rbind",
  .multicombine = TRUE,
  .packages = "Rpandemic"
  )%dopar% {
  
  result <- lockown_run(
    days = days,
    population.params = population.params,
    virus.params = virus.params,
    lockdown.sensitivity = trigger,
    lockdown.aggressiveness = severity
  )
  
  run_outcome <- result$epidemic_ts
  
  run_outcome$lockdown_degree <- severity
  run_outcome$lockdown_trigger <- trigger
  run_outcome$final_infections <- result$final_infected
  run_outcome$final_deaths <- result$final_dead
  
  return(run_outcome)
  
  
}



lockdown_results$infected.pct <- lockdown_results$infected / pop.size
infected_pct <- ggplot(lockdown_results[lockdown_results$lockdown_trigger == pop.size * .1,], aes(x=day, y=infected.pct, color=as.factor(lockdown_degree))) +
  scale_y_continuous(labels = percent) +
  scale_color_viridis(discrete = TRUE, direction = -1) +
  geom_line()

cumu_death <- ggplot(lockdown_results[lockdown_results$lockdown_trigger == pop.size * .1,], aes(x=day, y=cumu_death, color=as.factor(lockdown_degree))) +
  scale_y_continuous(labels = comma) +
  scale_color_viridis(discrete = TRUE, direction = -1) +
  geom_line()

total_infections<- ggplot(lockdown_results, aes(x=as.factor(lockdown_degree), y=final_infections, fill=as.factor(lockdown_trigger / pop.size))) +
  scale_fill_viridis(discrete = TRUE, direction = -1) +
  stat_summary(fun.y = "max", geom = "bar", position = "dodge")

setwd("results/severity_compare")


svg("infected-percent.svg")
print(infected_pct)
dev.off()

svg("cumu_dead.svg")
print(cumu_death)
dev.off()

svg("total_infected.svg")
print(total_infections)
dev.off()

write.csv(lockdown_results, "outcome-data.csv", row.names = FALSE)
