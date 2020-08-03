library(doParallel)
library(foreach)
library(ggplot2)
library(viridis)



isRStudio <- Sys.getenv("RSTUDIO") == "1"
if (isRStudio){
  N_CORES <- 3
} else{
  N_CORES <- 4
}
registerDoParallel(N_CORES)

total_run_time <- 360
N_ITER <- 10
update_interval <- 7

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

lockdown_sensitivity <- population.params$pop_size * 2e-5
# lockdown_sensitivity <- 3
new_sociabilities <- c(2, 5, 10, 15)

comparison_results <- foreach(
  i = 1:length(new_sociabilities),
  .combine = "rbind",
  .multicombine = TRUE
) %:% foreach(
  j = 1:N_ITER,
  .combine = "rbind",
  .multicombine = TRUE,
  .packages = c("Rpandemic", "magrittr")
) %dopar% {
  test_world <- smartSim(
    population.params = population.params,
    virus.params = virus.params
  ) 
  
  date <- 1
  
  while (date < total_run_time) {
    test_world %<>% runSim(days = update_interval)
    
    if (test_world$number_infections[date] >= lockdown_sensitivity) {
      test_world %<>% update(new.sociability = new_sociabilities[i])
      break
    }
    date <- date + update_interval 
  }
  
  
  if ((days_remaining <- total_run_time - date) >= 0)
    test_world %<>% runSim(days = days_remaining)
  
  output <- data.frame(
    date = as.Date(test_world$date, origin = "2020-01-22"),
    infections = test_world$number_infected,
    sociability = new_sociabilities[i],
    run_index = j
  )
}

comparison_results <- within(comparison_results, run_key <- paste(run_index, sociability, sep = "_"))

outplot <- ggplot(comparison_results, aes(x=date, y=infections, color=as.factor(sociability), group=run_key)) +
  geom_line() +
  scale_y_log10() +
  scale_color_viridis(discrete = TRUE)


if (!isRStudio) 
  svg("results/sociability_compare/lockdown-level_comparison.svg")
print(outplot)
if (!isRStudio)
  dev.off()
