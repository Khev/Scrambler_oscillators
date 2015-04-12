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
#include "funcs.h"


using namespace std;

vector<vector<double> > solve_odes_new(double S0, double gamma, double k, int num_osc, double T, int seed, string dir )
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


	//Instantiate
	double time_till_next_fire;
	vector< vector<double> > sols;
	vector<double> fired_neurons;
	double clock = 0.0;         		  // to help with recording times, see last step of program
	sols.reserve(int(1.2*num_osc));      	//first guess at num time steps
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


		//Do dynamics
		time_till_next_fire = fire_pulse(sols[t-1], sols[t],fired_neurons,S0,gamma,num_osc);
		scramble(sols[t], fired_neurons, num_osc);
		absorb(sols[t], fired_neurons, k,  num_osc);


		//Write to file
		sim_results << sols[t][0];
		for(int osc=1;osc<num_osc;osc++)
			sim_results << "," << sols[t][osc];
		sim_results << endl;

		
		//Clear vectors
		fired_neurons.erase(fired_neurons.begin(), fired_neurons.end());


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



