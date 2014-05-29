#ifndef __central_pattern_generator__RungeKutta__
#define __central_pattern_generator__RungeKutta__

#include <vector>
#include "Neuron.h"


#define WEIGHT_OFFSET 2

class CentralPatternGenerator;

class RungeKutta {
public:
    RungeKutta();
    RungeKutta(std::vector<Neuron>* network, std::vector<std::pair<int, int>> p, int cpg_index, std::array<CentralPatternGenerator, 11> * cpg_arr);
    void calcMeanMembranePotential(Neuron& n_old, Neuron& n_new, double, double);
    void calcFiringFrequency(Neuron& n_new);
    void step(Neuron& n, double timestep);
    double calcDmDt(Neuron& n);
    double calcDxDt(Neuron& n);
    double addWeightedNeighbors(Neuron& n);
    void updateNetwork(std::vector<Neuron>* network);
private:
    std::vector<std::pair<int, int>>  m_syn_spread_weights;
    std::vector<Neuron>* m_cur_network;
    double calcDerivative(double time, double m, double sigma, double tau);
    double findDivAmt(int index);
    
    int m_cpg_index;
    std::array<CentralPatternGenerator, 11> * m_cpg_arr;
};

#endif /* defined(__central_pattern_generator__RungeKutta__) */
