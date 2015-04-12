#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;


void scramble(vector<double> &sols_t, vector<double> fired_neurons, int num_osc)
	{

	/*
	"Scrambles" the oscillators -- each oscillators is given a new voltage uniformly at random.
	If two or more oscilaltors are sync'd in a 'cluster', they all receive the same new voltage.
	In this way, clusters are preserved under the scrambling procedure.


	Input: (i)  sols_t -- list of voltages at t^th timestep
				 (ii) fired_neurons -- list of oscillators (indices) that fired


	Output: sols_t is passed by reference => modified by function. 
	*/
		

		//Instantiate
		vector<double> unique_voltages;
		vector<double> reassigned_voltages;
		bool checker;
		bool isPresent;

		for(int osc=0;osc<num_osc;osc++){

			//Only Reassign neurons that haven't fired
			isPresent = (find(fired_neurons.begin(), fired_neurons.end(), osc) != fired_neurons.end());
			if(isPresent!=1){
			

				//Check that voltage hasn't been recored yet
				checker = 1;
				for(int v=0;v<unique_voltages.size();v++){
					if(sols_t[osc] == unique_voltages[v]){   	//make sure not in list already
						sols_t[osc] = reassigned_voltages[v];    //if he is in list already, then
						checker = 0;															//he's part of a synchronous pack,
						}																					//so give him the same reassignment
					}																						//as his friends


				//If not part of pack, then add to list & scramble!
				if(checker == 1){
					double temp = (double(rand()) / (double(RAND_MAX)+1));
					//cout << "temp = " << temp << ",";
					unique_voltages.push_back(sols_t[osc]);			//record the voltage to be reassigned
					reassigned_voltages.push_back(temp);    			//record what he got reassigned to
					sols_t[osc] = temp;													//do the reassignment
					}
				}
			}
	}




