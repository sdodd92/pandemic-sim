#include <entitymodule.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	size_t *n_dead, *n_infected, *new_infections, *n_recovered;
	size_t n_days;
} PandemicReport;

PandemicReport init_report(size_t n_days);

void write_output(FILE* fout, const PandemicReport* report);

// forward-declare pointers to sub-populations
void *population, *family, *work, *social;

// forward-declare subroutines to run aggregate time-steps
void step_week(int *date, PandemicReport *report);

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
	const double MORTALITY_RATE = .02;
	const int DISEASE_LENGTH = 31;
	const int LATENCY = 2;

	long n_families;
	long n_offices = POP_SIZE / WORK_SIZE;


int main(int argc, char** argv) {

	n_offices = POP_SIZE / WORK_SIZE;

	defineFortranPop(&population, &POP_SIZE);

	// TODO change this to guarantee exclusivity
	defineFortranSubPops(&work, &population, &n_offices, &WORK_SIZE);

	defineFortranSubPops(&social, &population, &N_COMMUNITIES, &SOCIABILITY);

	defineFortranFamilies(&family, &population, &n_families, &FAMILY_SIZE);

	PandemicReport report = init_report(N_DAYS);	
	
	int date = 0;
	int infection_index = 0;

	initFortranInfection(&population, &CONTAGIOUSNESS, &MORTALITY_RATE, &DISEASE_LENGTH, &LATENCY, &date, &infection_index); 
	
	is_weekend = false;
	while (date < N_DAYS) {
		step_week(&date, &report);

	}

	FILE *output = fopen("run-result.csv", "w");
	write_output(output, &report);	
	fclose(output);

//	puts("DATE,NUM_INFECTED,NEW_INFECTIONS,NUM_DEAD\n");
//	for (int i=0;i < report.n_days;++i)
//		printf("%d,%lu,%lu,%lu\n",i, report.n_infected[i], report.new_infections[i], report.n_dead[i]);

	return 0;
}

void step_week(int *date, PandemicReport *report) {
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

	long n_dead_subpop, n_dead_families, new_infections_subpop, new_infections_families, n_recovered_families, n_recovered_subpop;
	
	for (int i=0;i<n_steps & *date < N_DAYS;++i) {
		mingleFortranPop(&population, &key_subpop, &n_subpops, date, &new_infections_subpop, &n_dead_subpop, &n_recovered_subpop);
		mingleFortranPop(&population, &family, &n_families, date, &new_infections_families, &n_dead_families, &n_recovered_families);

		report->n_infected[*date] = getCurrentInfected(&population);
		report->n_dead[*date] = n_dead_subpop + n_dead_families;
		report->new_infections[*date] = new_infections_subpop + new_infections_families;
		report->n_recovered[*date] = n_recovered_subpop + n_recovered_families;

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

