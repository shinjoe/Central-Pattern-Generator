#ifndef __central_pattern_generator__CentralPatternGenerator__
#define __central_pattern_generator__CentralPatternGenerator__

#include <array>
#include <vector>
#include "Neuron.h"
#include "RungeKutta.h"

#define CAPTURE_SIZE 100

class CentralPatternGenerator {
public:
    CentralPatternGenerator();
    void initNet(std::vector<std::vector<double>>& chromosome);
    void run();
    double calcFitness();
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
    
    std::array<double, CAPTURE_SIZE> m_last_few_points;
    
    std::vector<Neuron> m_network;
    std::vector<Neuron> m_copy;
    
    std::vector<Neuron> * m_cur;
    std::vector<Neuron> * m_prev;
    
    bool m_using_network_one;
    
    
    RungeKutta m_solver;
    
    
};


#endif /* defined(__central_pattern_generator__CentralPatternGenerator__) */
