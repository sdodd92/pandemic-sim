#include <pandemicentities.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <getopt.h>

void write_results(std::ostream& os, unsigned int n_iter, unsigned long* infected, unsigned long* died, long* new_infections);
void write_network(std::ostream&, Population&);
void write_infection_log(std::ostream&, Population&);
void write_subcommunities(std::ostream&, Population&);

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

	//declare network output file
	std::string network_file;
	bool network_out = false;

	//declare infection log output file
	std::string log_file;
	bool log_out = false;
	int log_interval = 7;
	std::ofstream log;	
	
	std::string subcomm_file;
	bool subcomm_out = false;
	std::ofstream subcomm_output;

	//read command-line options
	int c;
	 while ((c = getopt(argc, argv, "s:c:r:C:M:I:D:L:n:N:O:W:l:i:S:")) != -1)
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
				case 'W':
				 	 network_file.assign(optarg);
				 	 network_out = true;
				 	 break;
				case 'S':
				 	 subcomm_file.assign(optarg);
				 	 subcomm_out = true;
				 	 subcomm_output = std::ofstream(subcomm_file);
				 	 break;
				case 'l':
				 	 log_file.assign(optarg);
				 	 log_out = true;
				 	 log = std::ofstream(log_file);
				 	 break;
				case 'i':
				 	 std::sscanf(optarg, "%d", &log_interval);
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
	population.random_structure(sociability);
		
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
	 
    		//std::cout << date % log_interval << "\n\n";
			if (log_out & date % log_interval == 0) 
				population.update_infection_log(date);
				
			date++;
	 }
	 
	 
        
	if (fileout) {
		std::ofstream output(outfile);
		write_results(output, n_iter, infected, died, new_infections);	
	} else {
		write_results(std::cout, n_iter, infected, died, new_infections);
	}

	if (network_out) {
		
		std::ofstream output(network_file);
		write_network(output, population);

	}
	
	if (log_out) 
		write_infection_log(log, population);	
		
	if (subcomm_out)
		write_subcommunities(subcomm_output, population);

	
	

	return 0;
}

void write_results(std::ostream& os, unsigned int n_iter, unsigned long* infected, unsigned long* died, long* new_infections) {
	
	os << "date,currently_infected,newly_infected,died\n";

	for (int i=0; i < n_iter; ++i) {
		
		os << i << "," << infected[i] << "," << new_infections[i] << "," << died[i] << "\n";	
	}

}

void write_network(std::ostream& output, Population& population) {

	Population::Network network = population.get_person_network();

	output << "source,target,weight\n";
	for (auto& node_from : network)
		for (auto& node_to : node_from.second) 
			output << node_from.first << ',' << node_to.first << ',' << node_to.second << '\n';



}


void write_infection_log(std::ostream& output, Population& population) {
	Population::InfectionLog input = population.get_infection_log();
	
	output << "date,uid,value\n";
	for (auto& level_1 : input)
		for (auto& level_2 : level_1.second)
			output << level_1.first << ',' << level_2.first << ',' << level_2.second << '\n';
}

void write_subcommunities(std::ostream& output, Population& population) {
	Population::MembershipNode input = population.get_community_memberships();
	
	output << "community,member\n";
	for (auto& community : input)
		for (auto& member : community.second)
			output << community.first << ',' << member << '\n';
}



