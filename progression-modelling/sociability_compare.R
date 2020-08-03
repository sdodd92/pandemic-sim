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

total_run_time <- 20
N_ITER <- 2

population.params <- expand.grid(
  sociability = c(15, 20),
  pop_size = 1e6,
  resistance = .5,
  compliance = 1.
)

virus.params <- list(
  contagiousness = .5,
  mortality_rate = .05,
  incubation_period =7,
  latent_period = 3,
  disease_length = 14
)

comparison_results <- foreach(
  i = 1:nrow(population.params),
  .combine = "rbind",
  .multicombine = TRUE
) %:% foreach(
  j = 1:N_ITER,
  .combine = "rbind",
  .multicombine = TRUE,
  .packages = c("Rpandemic", "magrittr")
) %dopar% {
  pop.params <- population.params[i, ]
  test_world <- smartSim(
    population.params = pop.params,
    virus.params = virus.params
  ) %>% 
    runSim(days = total_run_time)
  
  output <- data.frame(
    date = as.Date(test_world$date, origin = "2020-01-22"),
    infections = test_world$number_infected,
    sociability = pop.params$sociability,
    run_index = j
  )
}

comparison_results <- within(comparison_results, run_key <- paste(run_index, sociability, sep = "_"))

outplot <- ggplot(comparison_results, aes(x=date, y=infections, color=as.factor(sociability), group=run_key)) +
  geom_line() +
  scale_y_log10() +
  scale_color_viridis(discrete = TRUE)


if (!isRStudio) 
  svg("results/sociability_compare/sociability-comparison.svg")
print(outplot)
if (!isRStudio)
  dev.off()
