#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iterator>


double find_dist(double time, double x0,  double S0, double gamma);

using namespace std;


void fire_pulse_with_delay(vector<double> sols_t_previous, vector<double> &sols_t, vector< vector<double> > &log_of_fired_neurons, vector<double>  &pulse_times, double t_next_fire, double clock,  double tau,  double S0, double gamma,int num_osc)
	{

	/*
	Does firing procedure:

	1. Move everyone up their voltage curves.
	2. Record who fires, and reset their voltage.
	3. Add (t_next_pulse, fired_neurons) to log.


	Input: (i)  sols_t_previous -- list of voltages from previous timestep
				 (ii) sols_t -- list of voltages at current timestep
		
	Output: sol_t is passed by reference => modifed by function
					fired_neurons is returned -- list of oscillators who fired.
	*/
		

	//Move all neurons up and count how many fire
	vector<double> fired_neurons;
	double time_till_next_fire = t_next_fire - clock;
	for(int i=0;i<num_osc;i++){
		sols_t[i] = find_dist(time_till_next_fire, sols_t_previous[i], S0, gamma);
		if(sols_t[i] >= 1){
			fired_neurons.push_back(i);  //record who fired
			sols_t[i] = 0;              //reset voltage if fired
			}
		}
	log_of_fired_neurons.push_back(fired_neurons);
	pulse_times.push_back(tau + clock);
	}




// Auxilliary functions
double find_dist(double time, double x0, double S0, double gamma){
	if(gamma == 0)
		return S0*time + x0;
	else
		return S0 / gamma + (exp(-gamma*time) / gamma) * (gamma*x0 - S0);
	}


