#include <vector>
#include <string>
using namespace std;


double fire_pulse(vector<double> sol_t_previous, vector<double> &sols_t, vector<double> &fired_neurons,  double S0, double gamma, int num_osc);

void scramble(vector<double> &sols_t, vector<double> fired_neurons, int num_osc);

void absorb(vector<double> &sols_t, vector<double> &fired_neurons, double k, int num_osc);

vector<vector<double> > solve_odes_new(double S0, double gamma, double k, int num_osc, double T, int seed, string dir);

vector<vector<double> > solve_odes(double S0, double gamma, double k,  int num_osc, double T, int seed, string dir);

vector<vector<double> > solve_odes_ensemble(double S0, double gamma, int num_osc, double T, 			int seed, int trial);

vector<vector<vector<double> > > solve_odes_ensemble1(double S0, double gamma, int num_osc, double T, int seed, int trial);

vector<vector<double> > solve_odes_vary_k(double S0, double gamma, double k,  int num_osc, double T, int seed);

double** solve_odes_forward_euler(double S0, double gamma, double k, int num_osc, double dt, int NT, int seed, int trial);


vector<vector<double> > count_firing_rates(double S0, double gamma, double k,  int num_osc, double T, int seed);


double** find_clusters(vector< vector<double> > sols, int NT, int num_osc, string dir);


void find_light_cone(vector< vector<double> > sols, int NT, int num_osc, string dir);

void find_and_write_clusters(vector< vector<double> > sols, int NT, int num_osc, int max_cluster , string dir);

double** find_cluster_moments(double** clusters, int num_moments, int NT, int num_osc, string dir);

double** bin_clusters(double** clusters, vector<double> fire_times, double dt, int NT_strobe, int NT_clean, int num_osc);

int make_voltage_histogram(const vector<vector<double> >& sols, int num_osc);

