#include "RungeKutta.h"
#include "math.h"

using namespace std;

RungeKutta::RungeKutta() {}

RungeKutta::RungeKutta(vector<Neuron> * network, vector<pair<int, int>> * p) {
    m_cur_network = network;
    m_syn_spread_weights = p;
}

void RungeKutta::updateNetwork(vector<Neuron> * network) {
    m_cur_network = network;
}

double RungeKutta::addWeightedNeighbors(Neuron& n) {
    double sum = 0.0;
    vector<double> weights = n.getWeights();
    
    for (int i = 0; i < m_cur_network->size(); i++) {
        // there is a weight offset of 2 because the metadata we are
        // interested in starts 2 into the array
        // 0th and 1st are Tau and B
        sum += weights[i + WEIGHT_OFFSET] * (*m_cur_network)[i].getX();
    }
    
    return sum;
}

double RungeKutta::calcDerivative(double time, double m, double sigma, double tau) {
    return  (-m + sigma)/tau;
}



void RungeKutta::calcMeanMembranePotential(Neuron& n_old, Neuron& n_new, double time, double timestep) {
    if (n_old.getType() == BRAINSTEM) return;
    
    double m = n_old.getM();
    double tau = n_old.getWeights()[0];
    double sigma = addWeightedNeighbors(n_old);
    double k1 = calcDerivative(time, m, sigma, tau);
    double k2 = calcDerivative(time + .5*timestep, m + (timestep/2)*k1, sigma, tau);
    double k3 = calcDerivative(time + .5*timestep, m + (timestep/2)*k2, sigma, tau);
    double k4 = calcDerivative(time + timestep, m + timestep*k3, sigma, tau);
    
    double newVal = m + (1.0/6)*timestep*(k1 + 2*k2 + 2*k3 + k4);
    n_new.setM(newVal);
    
}

void RungeKutta::calcFiringFrequency(Neuron& n_new) {
    if (n_new.getType() == BRAINSTEM) return;
    n_new.setX(1.0/(1 + exp(n_new.getM() + n_new.getWeights()[1])));
}















