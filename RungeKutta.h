#ifndef __central_pattern_generator__RungeKutta__
#define __central_pattern_generator__RungeKutta__

#include <vector>
#include "Neuron.h"

#define WEIGHT_OFFSET 2

class RungeKutta {
public:
    RungeKutta();
    RungeKutta(std::vector<Neuron *>& network);
    void calcMeanMembranePotential(Neuron& n, double, double);
    void calcFiringFrequency(Neuron& n);
    void step(Neuron& n, double timestep);
    double calcDmDt(Neuron& n);
    double calcDxDt(Neuron& n);
    double addWeightedNeighbors(Neuron& n);
private:
    std::vector<Neuron *> m_network;
    double calcDerivative(double time, double m, double sigma, double tau);
};

#endif /* defined(__central_pattern_generator__RungeKutta__) */
