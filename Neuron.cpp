#include "Neuron.h"

Neuron::Neuron() {}

Neuron::Neuron(std::vector<double>& w, double m, double x) {
    m_weights = w;
    m_m = m;
    m_x = x;
}
