#include "Neuron.h"

using namespace std;

Neuron::Neuron() { }

Neuron::Neuron(std::vector<double>& w, double m,  double x, NEURON_TYPE n, string name, int network_index) {
    m_weights = w;
    m_m = m;
    m_x = x;
    m_ntype = n;
    m_name = name;
    m_network_index = network_index;
}

vector<double>& Neuron::getWeights() { return m_weights; }

void Neuron::setM(double m) { m_m = m; }

void Neuron::setX(double x) { m_x = x; }

double Neuron::getM() { return m_m; }

double Neuron::getX() { return m_x; }

string& Neuron::getName() { return m_name; }

NEURON_TYPE Neuron::getType() { return m_ntype; }

int Neuron::getNetworkIndex() { return m_network_index; }