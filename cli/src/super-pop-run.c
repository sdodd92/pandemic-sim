#include <entitymodule.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

typedef struct {
	size_t *n_dead, *n_infected, *new_infections, *n_recovered;
	size_t n_days;
} PandemicReport;

PandemicReport init_report(size_t n_days);

void write_output(FILE* fout, const PandemicReport* report);

// forward-declare pointers to sub-populations
void *family, *work, *social;

// forward-declare subroutines to run aggregate time-steps
void step_week(int *date, PandemicReport *report, Population population);

bool is_weekend;

// TODO change this to cmd params
	const size_t POP_SIZE = 1000000;
	const double COMPLIANCE = 0.5;
	const double RESISTANCE = 0.5;

	const int SOCIABILITY = 20;
	const int WORK_SIZE = 100;
	const int FAMILY_SIZE = 4;

	const size_t N_COMMUNITIES = 200000;

	const size_t N_DAYS = 30;

	const double CONTAGIOUSNESS = .5;
	const double MORTALITY_RATE = 0;
	const int DISEASE_LENGTH = 31;
	const int LATENCY = 2;

	long n_families;
	long n_offices = POP_SIZE / WORK_SIZE;


int main(int argc, char** argv) {

	int n_threads = argc > 1 ? atoi(argv[1]) : 1;
	omp_set_num_threads(n_threads);

	n_offices = POP_SIZE / WORK_SIZE;

	Population population = newPopulation(POP_SIZE);

//	defineFortranPop(&population, &POP_SIZE);

	// TODO change this to guarantee exclusivity
	allocateSubPops(&work, population, &n_offices, &WORK_SIZE);

	allocateSubPops(&social, population, &N_COMMUNITIES, &SOCIABILITY);

	allocateFamilies(&family, population, &n_families, &FAMILY_SIZE);

	PandemicReport report = init_report(N_DAYS);	
	
	int date = 0;
	int infection_index = 0;

	initiateInfection(population, &CONTAGIOUSNESS, &MORTALITY_RATE, &DISEASE_LENGTH, &LATENCY, &date, &infection_index); 
	
	is_weekend = false;
	while (date < N_DAYS) {
		step_week(&date, &report, population);

	}

//	FILE *output = fopen("run-result.csv", "w");
//	write_output(output, &report);	
//	fclose(output);
//
	puts("DATE,NUM_INFECTED,NEW_INFECTIONS,NUM_DEAD\n");
	for (int i=0;i < report.n_days;++i)
		printf("%d,%lu,%lu,%lu\n",i, report.n_infected[i], report.new_infections[i], report.n_dead[i]);

	freePopulation(population);

	return 0;
}

void step_week(int *date, PandemicReport *report, Population population) {
	short n_steps;
	void *key_subpop;
	long n_subpops;
	
	if (is_weekend) {
		key_subpop = social;
		n_steps = 2;
		n_subpops = N_COMMUNITIES;
		is_weekend = false;  //toggle weekend value for next call
	} else {

		key_subpop = work;
		n_steps = 5;
		n_subpops = n_offices;
		is_weekend = true;
	}

	long n_dead, n_recovered, new_infections_subpop, new_infections_families;
	
	for (int i=0;i<n_steps & *date < N_DAYS;++i) {
		// both sets of subpopulations mingle BEFORE the health update is computed
		minglePopulation(population, &key_subpop, &n_subpops, date, &new_infections_subpop);
		minglePopulation(population, &family, &n_families, date, &new_infections_families);

//		report->n_infected[*date] = getCurrentInfected(population);


		// separately run the health update after all the day's mingling has occurred
		updatePopulation(population, date, &n_dead, &n_recovered);

		
		report->new_infections[*date] = new_infections_subpop + new_infections_families;
		report->n_dead[*date] = n_dead;
		report->n_recovered[*date] = n_recovered;

		report->n_infected[*date] = getCurrentInfected(population);

		*date += 1;
	}

}

void write_output(FILE* fout, const PandemicReport* report) {
	fprintf(fout, "DATE,NUM_INFECTED,NEW_INFECTIONS,NUM_DEAD\n");
	for (int i=0;i < report->n_days;++i)
		fprintf(fout, "%d,%lu,%lu,%lu\n",i, report->n_infected[i], report->new_infections[i], report->n_dead[i]);
		
}

PandemicReport init_report(size_t n_days) {

	PandemicReport new_report;
	new_report.n_days = n_days;
	new_report.n_dead = malloc(n_days * sizeof(size_t));
	new_report.n_infected = malloc(n_days * sizeof(size_t));
	new_report.new_infections = malloc(n_days * sizeof(size_t));
	new_report.n_recovered = malloc(n_days * sizeof(size_t));

	return new_report;

}

