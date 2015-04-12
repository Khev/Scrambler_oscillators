#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include <fstream>

using namespace std;


double** find_clusters(vector<vector<double> > sols, int NT, int num_osc, string dir){	
	/*  Finds c_i(t)  :=  N_i / N,
			N_i = num of groups of sync'd oscillators of size i. I.e. if 
			there are 7 pairs of oscillators synchronised, then 
			N_2 = 7
	*/


	//Open file
	ofstream fout;
	if(dir == "default")
		fout.open("cluster_results.csv");
	else
		fout.open("./" + dir + "/cluster_results.csv");
		

	//Instantiate
	double** clusters;
	clusters = new double*[NT];
	for(int t=0;t<NT;t++){
		clusters[t] = new double[num_osc];
		for(int osc=0;osc<num_osc;osc++)
			clusters[t][osc] = 0;
		};


	//Find Clusters  -- c_j(t) as defined in paper
	for(int t=0;t<NT;t++){
		
		//Find osc(t)
		double osc[num_osc];   
		for(int i=0;i<num_osc;i++)
			osc[i] = sols[t][i];

		//Tally data -- sort, then count duplicates, and record
		sort(osc, osc+sizeof(osc)/sizeof(double));
		
		int counter = 0;
		for(int i=0;i<num_osc-1;i++){	
			if(osc[i] == osc[i+1])
				counter++;
			else{
				counter++;
				clusters[t][counter-1] += 1 / double(num_osc);
				counter = 0;
			  }
			}
			
			//Boundary Case
			counter += 1;
			clusters[t][counter-1] += 1 / double(num_osc);
			
			/* I'm changing writing to file slightly to
				that it only writes the first 5
			*/

			//Write to file
			//int max_cluster_to_print = 5;
			fout << clusters[t][0];
			for(int i=1;i<num_osc;i++)
				fout << "," << clusters[t][i];
			fout << endl;

		}

		fout.close();

	
	return clusters;
	}
   
		





		
	
	
