#include <iostream>
#include "funcs.h"
#include <ctime>
#include <vector>
#include <ctime>
#include <fstream>
#include <time.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]){

	/* 

		Command line arguments: Default # of arguements is 1, see (i) below.
		Can specify an optinal third argument, see (ii) below.
		
		(i) int Num Osc

		(ii) int Num Osc, string "subdirectory" -- name of sub_dir to 
				store the .csv files (output)

		(iii) int Num_Osc, string 'subdirectory', int seed -- seed the random # generator.	Note, the directory has to exist before writing to it.

	*/


	//Parameters
	double S0 = 1;
	double gamma = 0;
	double k = 1;
	double T = 10;
	int num_osc = atoi(argv[1]);
	int num_moments = 5;
	srand(time(NULL)); int seed = rand();
	string dir = "default";
	

	//Make seed an optional 3rd argument
	if(argc == 4){
		dir = argv[2];
		seed = atoi(argv[3]);
		}

	if(argc == 3)
		dir = argv[2];
		

	//Run simulation
	clock_t begin = clock();
	vector<vector<double> > sols = solve_odes_new(S0, gamma, k, num_osc,T, seed, dir);
	int NT = sols.size();
	double** clusters = find_clusters(sols, NT, num_osc, dir);
	double** moments = find_cluster_moments(clusters, num_moments, NT, num_osc, dir);
	clock_t end = clock();
		

	//Print Timing
	cout << "Sim with (N, T) = (" << num_osc <<  "," << T << ") "<< "  took t = "
	<< double (end - begin) / CLOCKS_PER_SEC << endl;
	

	return 0;
	}


