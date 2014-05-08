#ifndef __central_pattern_generator__RungeKutta__
#define __central_pattern_generator__RungeKutta__

#include <vector>
#include "Neuron.h"

class RungeKutta {
public:
    RungeKutta();
    RungeKutta(std::vector<Neuron> network);
    void calcMeanMembranePotential();
    void calcFiringFrequency();
private:
    std::vector<Neuron> m_network;
};

#endif /* defined(__central_pattern_generator__RungeKutta__) */
