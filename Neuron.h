#ifndef __central_pattern_generator__Neuron__
#define __central_pattern_generator__Neuron__

#include <iostream>
#include <vector>

#include "Chromosome.h"

class Neuron {
public:
private:
    Chromosome m_chromosome;
    std::vector<Neuron *> m_neighbors;
};

#endif /* defined(__central_pattern_generator__Neuron__) */
