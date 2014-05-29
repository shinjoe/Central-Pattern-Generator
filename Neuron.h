#ifndef __central_pattern_generator__Neuron__
#define __central_pattern_generator__Neuron__

#include <iostream>
#include <vector>
#include <string>

#include "Chromosome.h"

enum NEURON_TYPE {
    MOTO, INTER, BRAINSTEM
};


class Neuron {
public:
    Neuron();
    Neuron(std::vector<double>&, double m=1.0, double x=1.0, NEURON_TYPE n=INTER, std::string name="UNASSIGNED", int network_index=-1);
    double getM();
    double getX();
    void setM(double m);
    void setX(double x);
    int getNetworkIndex();
    std::string& getName();
    NEURON_TYPE getType();
    std::vector<double>& getWeights();
private:
    Chromosome m_chromosome;
    std::string m_name;
    std::vector<double> m_weights;
    double m_m;
    double m_x;
    int m_network_index;
    NEURON_TYPE m_ntype;

};

#endif /* defined(__central_pattern_generator__Neuron__) */
