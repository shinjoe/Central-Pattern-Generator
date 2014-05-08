#include "Neuron.h"

using namespace std;

Neuron::Neuron() { }

Neuron::Neuron(std::vector<double>& w, double m,  double x, NEURON_TYPE n, string name) {
    m_weights = w;
    m_m = m;
    m_x = x;
    m_ntype = n;
    m_name = name;
}


double Neuron::getM() { return m_m; }

double Neuron::getX() { return m_x; }

string& Neuron::getName() { return m_name; }