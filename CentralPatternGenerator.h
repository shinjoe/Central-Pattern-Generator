#ifndef __central_pattern_generator__CentralPatternGenerator__
#define __central_pattern_generator__CentralPatternGenerator__

#include <vector>
#include "Neuron.h"
#include "RungeKutta.h"

class CentralPatternGenerator {
public:
    CentralPatternGenerator();
    void run();
private:
    
    Neuron A_left;
    Neuron B_left;
    Neuron C_left;
    Neuron M_left;
    Neuron BS_left;
    
    Neuron A_right;
    Neuron B_right;
    Neuron C_right;
    Neuron M_right;
    Neuron BS_right;
    
    std::vector<Neuron *> m_network;
    
    RungeKutta m_solver;
    
    
};


#endif /* defined(__central_pattern_generator__CentralPatternGenerator__) */
