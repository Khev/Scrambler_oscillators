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


using namespace std;



int rough_work()
	{

	double S0 = 1;
	double gamma = 0;
	double k = 1;
	int num_osc = 100;
	double T = 3;
	int seed = 1;
	string dir = "default"


	//Instantiate
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
		fired_neurons = fire_pusle(sols[t-1], sols[t],fired_neurons,S0,gamma,num_osc);
		scramble(sols[t], fired_neurons, num_osc);
		absorb(sols[t], fired_neurons, num_osc);


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


	return 1
	}



