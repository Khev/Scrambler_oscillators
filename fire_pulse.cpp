#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <vector>
#include <iterator>


double find_dist(double time, double x0,  double S0, double gamma);
double find_time(double dist, double x0, double S0, double gamma);

using namespace std;


double fire_pulse(vector<double> sols_t_previous, vector<double> &sols_t, vector<double> &fired_neurons, double S0, double gamma,int num_osc)
	{

	/*
	Does firing procedure:

	1. Find when next firing event is
	2. Move everyone up their voltage cuves
	3. Record who fires, and reset their voltage
	4. Return vector of oscillators who fired


	Input: (i)  sols_t_previous -- list of voltages from previous timestep
				 (ii) sols_t -- list of voltages at current timestep
		
	Output: sol_t is passed by reference => modifed by function
					fired_neurons is returned -- list of oscillators who fired.
	*/
		


	//Find when next firing event is

	int fire_index = distance(sols_t_previous.begin(), max_element(sols_t_previous.begin(), sols_t_previous.end()) );
	double time_till_next_fire = find_time(1, sols_t_previous[fire_index], S0, gamma);


	//Move all neurons up and count how many fire
	for(int i=0;i<num_osc;i++){
		sols_t[i] = find_dist(time_till_next_fire, sols_t_previous[i], S0, gamma);
		if(sols_t[i] >= 1){
			fired_neurons.push_back(i);  //record who fired
			sols_t[i] = 0;              //reset voltage if fired
			}
		}

	return time_till_next_fire;
	}




// Auxilliary functions
double find_dist(double time, double x0, double S0, double gamma){
	if(gamma == 0)
		return S0*time + x0;
	else
		return S0 / gamma + (exp(-gamma*time) / gamma) * (gamma*x0 - S0);
	}

double find_time(double x, double x0, double S0, double gamma){
	if(gamma == 0)
		return (1 - x0) / S0;
	else
		return (1 / gamma)* log( (gamma*x0 - S0) / ( gamma*x - S0));
	}

