#ifndef __central_pattern_generator__CentralPatternGenerator__
#define __central_pattern_generator__CentralPatternGenerator__

#include <array>
#include <vector>
#include "Neuron.h"
#include "RungeKutta.h"


#define CAPTURE_SIZE 400

class CentralPatternGenerator {
public:
    CentralPatternGenerator();
    void initNet(std::vector<std::vector<double>>& chromosome, std::vector<std::pair<int, int>> * pair_vec,
                 std::array<CentralPatternGenerator, 11> * cpg_arr, int index);
    void run();
    double calcFitness();
    double calcIntersegmentalFitness(CentralPatternGenerator& nextCpg);
    int findPeakIndex(int starting_index);
    double findXOf(int index);
    
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
    
    std::array<CentralPatternGenerator, 11> * m_cpg_arr;
    int m_index;
    
    std::vector<Neuron> m_network;
    std::vector<Neuron> m_copy;
    
    std::vector<Neuron> * m_cur;
    std::vector<Neuron> * m_prev;
    
    bool m_using_network_one;
    
    
    RungeKutta m_solver;
    
    
};


#endif /* defined(__central_pattern_generator__CentralPatternGenerator__) */
