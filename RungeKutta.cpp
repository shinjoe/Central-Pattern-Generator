#include "RungeKutta.h"
#include "math.h"
#include "CentralPatternGenerator.h"

using namespace std;

RungeKutta::RungeKutta() {}

RungeKutta::RungeKutta(vector<Neuron> * network, vector<pair<int, int>> p, int cpg_index, std::array<CentralPatternGenerator, 11> * cpg_arr) {
    m_cur_network = network;
    m_syn_spread_weights = p;
    m_cpg_index = cpg_index;
    m_cpg_arr = cpg_arr;
}

void RungeKutta::updateNetwork(vector<Neuron> * network) {
    m_cur_network = network;
}

int findIndex(int i) {
    // don't divide by weights for ml, bs_l, mr, and bs_r
    if (i == 0 || i == 4 || i == 5 || i == 9) return -1;
    else if (i == 1 || i == 2  || i == 3) return i - 1;
    else return i - 3;
}

double RungeKutta::findDivAmt(int i) {
    int index = findIndex(i);
    if (index == -1) return 1.0;
    pair<int, int> weights = m_syn_spread_weights[index];
    return weights.first + weights.second + 1;
}

double RungeKutta::addWeightedNeighbors(Neuron& n) {
    double sum = 0.0;
    vector<double> weights = n.getWeights();
    
    for (int i = 0; i < m_cur_network->size(); i++) {
        // there is a weight offset of 2 because the metadata we are
        // interested in starts 2 into the array
        // 0th and 1st are Tau and B
        double synaptic_spread_divider = findDivAmt(i);
        sum += weights[i + WEIGHT_OFFSET] * (*m_cur_network)[i].getX() / synaptic_spread_divider;
    }
    
    int netIndex = n.getNetworkIndex();
    if (netIndex != -1 && netIndex != 0 && netIndex != 5) {
        pair<int, int> bounds = m_syn_spread_weights[findIndex(n.getNetworkIndex())];
        double neuron_div_amt = bounds.first + bounds.second + 1;
        
        for (int j = m_cpg_index - bounds.second; j < m_cpg_index + bounds.first; j++) {
            if (j < 0 || j > 10) continue;
            sum +=  (*m_cpg_arr)[j].findXOf(n.getNetworkIndex()) / neuron_div_amt;
        }
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















