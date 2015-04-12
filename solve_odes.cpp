#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

double find_dist(double time, double x0,  double S0, double gamma);
double find_time(double dist, double x0, double S0, double gamma);

using namespace std;

vector<vector<double> > solve_odes(double S0, double gamma, double k, int num_osc, double T, int seed, string dir )
	{
	/*
		 Solves the equations dx_i/ dt = S0 - gamma*x_i
		 where i = [1,num_osc]

		 "seed" is given as an argument to help with the random number generation when
		 	taking ensembles of simulations.

			'k' is the size of the sync zone; the pulses are of size k*j / N is, where j is
			the # of oscillators that fire. (For paul's theory, k = 1)

			The difference between solve_odes.cpp and quick_solve_odes.cpp is that the
			former numerically integrates the odes, while the latter exploits the fact
			that the ODE's have an analytic solution. The algorithm is,

			1. Analyically find which neuron is first to fire, and how long this will take.
			2. Move every other neuron up
			3. Do fire conditon
			4. Repeat.

			This is superior in that there are no issues with timesteps, and unnecessary
			times where no neurons are firing aren't recorded. The number of timesteps isn't
			known a priori, which means that arrays can't be used. So, must use vectors instead
	*/
		
	vector< vector<double> > sols;
	double clock = 0.0;         		  // to help with recording times, see last step of program
	sols.reserve(int(1.2*num_osc));      	//first guess at num time steps


	//For Better Random Number Generation
	//default_random_engine generator(seed);
  //uniform_real_distribution<double> distribution(0.0,1.0);

	//Here I'm trying an alternate random number generation, for compatibility
	// reasons with the Sethna cluster
	srand(time(NULL));
	
	
	//Open file to write to
	ofstream sim_results;
	ofstream times;
	if(dir == "default"){
		sim_results.open("sim_results.csv");
		times.open("fire_times_N_" + to_string(num_osc) + ".csv");
		}
	else{
		sim_results.open("./" + dir + "/sim_results.csv");
		times.open("./" + dir + "fire_times_N_" + to_string(num_osc) + ".csv");
		}

		
	//Make IC
	vector<double> row;  // this contains the voltages at every timestep
	sols.push_back(row);
	sols[0].resize(num_osc);
	for(int i=0;i<num_osc;i++){
		sols[0][i] = ( double(rand()) /( double(RAND_MAX) + 1));
		if(i==0)                        
			sim_results << sols[0][i];
		else
			sim_results << "," << sols[0][i]; 
		}
	sim_results << endl;
	times << 0;



/* --------------------- START MAIN WHILE LOOP ------------------------------------------ */
	int t = 1;

	while(clock < T){
		//Instantiate
		sols.push_back(row);
		sols[t].resize(num_osc);  //allocate memory


		//Find which neuron nearest to firing
		double dist_nearest_neuron = 1;
		double first_fire_index = 0;
		for(int i=0;i<num_osc;i++){
			if(1 - sols[t-1][i] < dist_nearest_neuron)
				dist_nearest_neuron  = 1 - sols[t-1][i];
				first_fire_index = i;
			}

	
		// Move all neurons up and count how many fire
		double time_till_next_fire = find_time(dist_nearest_neuron, sols[t-1][first_fire_index], S0,gamma);

		vector<double> fired_neurons; 
		for(int i=0;i<num_osc;i++){
			sols[t][i] = sols[t-1][i] + find_dist(time_till_next_fire, sols[t-1][i], S0, gamma);
			if(sols[t][i] >= 1){
				fired_neurons.push_back(i);  //record who fired
				sols[t][i] = 0;              //reset voltage if fired
				}
			}
		

		//Scramble the oscillators, V_old -> V_new
		vector<double> unique_voltages;
		vector<double> reassigned_voltages;
		unique_voltages.resize(num_osc);
		reassigned_voltages.resize(num_osc);
		bool checker;
		bool isPresent;

		for(int osc=0;osc<num_osc;osc++){

			//Only Reassign neurons that haven't fired
			isPresent = (find(fired_neurons.begin(), fired_neurons.end(), osc) != fired_neurons.end());
			if(isPresent!=1){
			

				//Check that voltage hasn't been recored yet
				checker = 1;
				for(int v=0;v<unique_voltages.size();v++){
					if(sols[t][osc] == unique_voltages[v]){   	//make sure not in list already
						sols[t][osc] = reassigned_voltages[v];    //if he is in list already, then
						checker = 0;															//he's part of a synchronous pack,
						}																					//so give him the same reassignment
					}																						//as his friends


				//If not part of pack, then add to list & scramble!
				if(checker == 1){
					double temp = (double(rand()) / (double(RAND_MAX)+1));
					unique_voltages.push_back(sols[t][osc]);			//record the voltage to be reassigned
					reassigned_voltages.push_back(temp);    			//record what he got reassigned to
					sols[t][osc] = temp;													//do the reassignment
					}
				}
			}


		//Absorb anyone within the sync zone
		double sync_zone = ( k* double(fired_neurons.size()) ) / double(num_osc); // j/N
		for(int i=0;i<num_osc;i++){
		isPresent = (find(fired_neurons.begin(), fired_neurons.end(), i) != fired_neurons.end());
	    if(isPresent!=1){
				bool inZone;
				inZone = (1- sols[t][i]) < sync_zone;
				if(inZone == 1) 							//are they in the sync zone
					fired_neurons.push_back(i);		
				}
			}
			

		//Reset all neurons that have fired
		for(int osc=0;osc<fired_neurons.size();osc++)
			sols[t][fired_neurons[osc]] = 0;


		//Write to file
		sim_results << sols[t][0];
		for(int osc=1;osc<num_osc;osc++)
			sim_results << "," << sols[t][osc];
		sim_results << endl;

		
		//Clear vectors
		fired_neurons.erase(fired_neurons.begin(), fired_neurons.end());
		unique_voltages.erase(unique_voltages.begin(), unique_voltages.end()); 
		reassigned_voltages.erase(reassigned_voltages.begin(), reassigned_voltages.end()); 


		//While loop counters
		clock += time_till_next_fire;
		times << "," << clock;
		t++;
		}	
	

	//Close files
	sim_results.close();
	times.close();


	return sols;
	}


// Auxilliary functions
double find_dist(double time, double x0, double S0, double gamma){
	if(gamma == 0)
		return S0*time;
	else
		return x0*gamma*exp(-gamma*time) + (S0 / gamma) * (1 - exp(gamma*time));
	}

double find_time(double dist, double x0, double S0, double gamma){
	if(gamma == 0)
		return dist / S0;
	else
		return (1 / gamma)*log( (S0 - x0*gamma) / (S0 -1*gamma));
	}

