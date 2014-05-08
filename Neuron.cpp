#include "Neuron.h"

Neuron::Neuron() {}

Neuron::Neuron(std::vector<double>& w) {
    m_weights = w;
}
