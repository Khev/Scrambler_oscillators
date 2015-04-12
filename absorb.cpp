#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>


using namespace std;


void absorb(vector<double> &sols_t, vector<double> &fired_neurons, double k, int num_osc ){

	/*
	'Absorbs' all oscillators on the interval [1- k*j/N, 1) -- by resetting their voltage to 0 -- where
	 j is the size of the cluster that fired, and k is a parameter that characeterises the extent of the 
	 absorption.

	Input: (i) sol_t -- list of voltages at t^th timestep
				(ii) fired_neurons -- list of oscillators (indices) that fired
				(iii) k -- absorption distance parameter

	Output: sols_t is passed by reference => modified by function
					fired_neurons is passed by reference => modified by function

	*/
		

	//Instantiate
	bool isPresent;
	bool inZone;
	double sync_zone = ( k* double(fired_neurons.size()) ) / double(num_osc); // j/N

	
	for(int i=0;i<num_osc;i++){
		isPresent = (find(fired_neurons.begin(), fired_neurons.end(), i) != fired_neurons.end());
	   if(isPresent!=1){
			inZone = (1- sols_t[i]) < sync_zone;
			if(inZone == 1) 							//are they in the sync zone		
				sols_t[i] = 0;
			}
		}
	}



