#include <pandemicentities.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <getopt.h>

void write_results(std::ostream& os, unsigned int n_iter, unsigned long* infected, unsigned long* died, long* new_infections);

int main(int argc, char** argv) {

	// declare community parameters
	int sociability=-1;
	double compliance = 1;
	double resistance = 0;
	
	
	
	// declare virus parameters
	Pathogen* virus;
	double contagiousness = 0;
	double mortality_rate = 0;
	int incubation_period = 0;
	int disease_length = 0;
	int latent_period = 0;
	
	unsigned int n_iter = 0;
	unsigned long pop_size = 0;
	
	//declare output file name
	std::string outfile;
	bool fileout = false;
	
	//read command-line options
	int c;
	 while ((c = getopt(argc, argv, "s:c:r:C:M:I:D:L:n:N:O:")) != -1)
        switch (c) {
            case 's':
                std::sscanf(optarg, "%d", &sociability);
                break;
            case 'c':
                std::sscanf(optarg, "%lf", &compliance);
                break;
            case 'r':
                std::sscanf(optarg, "%lf", &resistance);
                break;
            case 'C':
            	std::sscanf(optarg, "%lf", &contagiousness);
            	break;
            case 'M':
            	 std::sscanf(optarg, "%lf", &mortality_rate);
            	 break;
            case 'I':
                std::sscanf(optarg, "%d", &incubation_period);
                break;
            case 'D':
                std::sscanf(optarg, "%d", &disease_length);
                break;
            case 'L':
                std::sscanf(optarg, "%d", &latent_period);
            case 'n':
            	 std::sscanf(optarg, "%u", &n_iter);
            	 break;
            case 'N':
            	 std::sscanf(optarg, "%lu", &pop_size);
                break;
            case 'O':
            	 outfile.assign(optarg);
            	 fileout = true;
            	 break;
            default:
                abort();
        }
        
    if (n_iter <= 0) {
		std::cout << "number of iterations must be a positive integer (got " << n_iter << ")\n" ;
		return 1;
    }
        
    //initialize world entities
    Population population(pop_size, compliance, resistance);
	population.define_structure(sociability);
		
	 virus = new Pathogen(
		   contagiousness,
    		mortality_rate,
    		disease_length,
    		incubation_period,
    		latent_period	 
	 );
	 
	 //initiate the infection
	 population.initiate_infection(virus, 0, 0); // arbitrarily pick the first person as patient zero
	 
	 
	 //initialize time-series trackers of deaths and infections
	 
	 unsigned long* infected = (unsigned long*)malloc(sizeof(unsigned long) * n_iter);
	 unsigned long* died = (unsigned long*)malloc(sizeof(unsigned long) * n_iter);
	 long* new_infections = (long*)malloc(sizeof(long) * n_iter);
	 
	 
	 infected[0] = 1; // we know one person is infected since we forced it
	 new_infections[0] = 1;
	 
	 int date = 0;  // TODO: decide if this is correct - are we losing info from day 0?
	 while (date < n_iter) {
	 	
	 		  new_infections[date] = population.mingle(date);
	 		
	 		  died[date] = population.update_health(date);

  			  infected[date] = population.get_num_infected();
	 
    		date++;
		 
	 }
	 
	 
        
	if (fileout) {
		std::ofstream Outfile(outfile);
		write_results(Outfile, n_iter, infected, died, new_infections);	
	} else {
		write_results(std::cout, n_iter, infected, died, new_infections);
	}

	
	

	return 0;
}

void write_results(std::ostream& os, unsigned int n_iter, unsigned long* infected, unsigned long* died, long* new_infections) {
	
	os << "date,currently_infected,newly_infected,died\n";

	for (int i=0; i < n_iter; ++i) {
		
		os << i << "," << infected[i] << "," << new_infections[i] << "," << died[i] << "\n";	
	}

}
