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
double find_time(double x, double x0, double S0, double gamma);

vector<vector<double> > solve_odes_with_delay(double S0, double gamma, double k, double tau, int num_osc, double T, int seed, string dir )
	{
	/*
	Same as old system (see solve_odes for details), except now there is a delay of tau seconds 
	between the transmission and reception of a pulse
	*/


	//Instantiate
	double t_next_pulse;
	double t_next_fire;
	int fire_index;
	vector<double> pulse_times;
	vector< vector<double> > log_of_fired_neurons; 
	vector< vector<double> > sols;
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


		//Find t_next_pulse & t_next_fire
		fire_index = distance(sols[t-1].begin(), max_element(sols[t-1].begin(), sols[t-1].end()));
		t_next_fire = clock + find_time(1, sols[t-1][fire_index], S0, gamma);
		if(pulse_times.empty() == true)
			t_next_pulse = T+1;  //if no pulse, make pulse time > T_total, so that fire_time < pulse_time 
		else;
			t_next_pulse = pulse_times[0];  


		//Firing event
		if(t_next_fire < t_next_pulse){
			fire_pulse_with_delay(sols[t-1], sols[t], log_of_fired_neurons, pulse_times, t_next_fire, clock,tau, S0, gamma, num_osc);
			
			//Write to file & update while loop counters
			sim_results << sols[t][0];
			for(int osc=1;osc<num_osc;osc++)
				sim_results << "," << sols[t][osc];
			sim_results << endl;
			clock = t_next_fire;
			times << "," << clock;
			t++;
			}

		
		//Pulse 
		else{
			scramble(sols[t], log_of_fired_neurons[0], num_osc);
			absorb(sols[t], log_of_fired_neurons[0], k, num_osc);
			log_of_fired_neurons.erase(log_of_fired_neurons.begin());
		
		
			//Write to file & update while loop counters
			sim_results << sols[t][0];
			for(int osc=1;osc<num_osc;osc++)
				sim_results << "," << sols[t][osc];
			sim_results << endl;
			clock = t_next_pulse;
			times << "," << clock;
			t++;
			}
		}	
	

	//Close files
	sim_results.close();
	times.close();

	return sols;
	}



double find_time(double x, double x0, double S0, double gamma){
	if(gamma ==0)
		return (1- x0) / S0;
	else
		return (1 / gamma) * log( (gamma*x0 - S0) / (gamma*x - S0) );
	}



