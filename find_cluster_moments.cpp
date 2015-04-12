#include <cmath>
#include <fstream>
#include <string>


double** find_cluster_moments(double** clusters, int num_moments, int NT, int num_osc, std::string dir){
	/* Find the moments of the clusters c_i(t) as defined in "find_cluster.cpp". I.e.
		 M0 = Sum(c_i, i)
		 M1 = Sum(i*c_i, i)
		 M2 = Sum(i^2*c_i, i) etc
	*/


	//Open file
	std::ofstream fout;
	if(dir == "default")
		fout.open("moment_results.csv");
	else
		fout.open("./" + dir + "/moments_results.csv");


	//Instantiate
	double** moments;
	moments = new double*[num_moments];
	for(int i=0;i<num_moments;i++){
		moments[i] = new double[NT];

		//Calculate Moments
		for(int t=0;t<NT;t++){
			double sum = 0;
			for(int osc=0;osc<num_osc;osc++)
				sum += pow(osc+1,i)*clusters[t][osc];
			moments[i][t] = sum;
			if(t==0)
				fout << sum;
			else
				fout << "," << sum;
			}
			fout << std::endl;
		}
		fout.close();

	return moments;
	}

