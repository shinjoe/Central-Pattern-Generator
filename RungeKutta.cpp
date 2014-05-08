#include "RungeKutta.h"
#include "math.h"

using namespace std;

RungeKutta::RungeKutta() {}

RungeKutta::RungeKutta(std::vector<Neuron> network) {
    m_network = network;
}

double RungeKutta::addWeightedNeighbors(Neuron& n) {
    double sum = 0.0;
    vector<double> weights = n.getWeights();
    
    for (int i = 0; i < m_network.size(); i++) {
        // there is a weight offset of 2 because the metadata we are
        // interested in starts 2 into the array
        // 0th and 1st are Tau and B
        sum += weights[i + WEIGHT_OFFSET] * m_network[i].getX();
    }
    
    return sum;
}

double RungeKutta::calcDerivative(double time, double m, double sigma, double tau) {
    return (-m + sigma)/tau;
}

void RungeKutta::calcMeanMembranePotential(Neuron& n, double time, double timestep) {
    if (n.getType() == BRAINSTEM) return;
    
    double m = n.getM();
    double tau = n.getWeights()[0];
    double sigma = addWeightedNeighbors(n);
    double k1 = calcDerivative(time, m, sigma, tau);
    double k2 = calcDerivative(time + .5*timestep, m + (timestep/2)*k1, sigma, tau);
    double k3 = calcDerivative(time + .5*timestep, m + (timestep/2)*k2, sigma, tau);
    double k4 = calcDerivative(time + timestep, m + timestep*k3, sigma, tau);
    n.setM(m + (1.0/6)*timestep*(k1 + 2*k2 + 2*k3 + k4));
    
}

void RungeKutta::calcFiringFrequency(Neuron& n) {
    if (n.getType() == BRAINSTEM) return;
    n.setX(1.0/(1 + exp(n.getM() + n.getWeights()[1])));
}















