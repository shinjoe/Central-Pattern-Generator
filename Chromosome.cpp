#include "Chromosome.h"
#include "GeneticAlgo.h"
#include <iostream>
#include <cmath>

#define TARGET 20
#define PARAM_LEN 12
#define NEURONS_TO_SET_UP 4
#define CONST_INDEX 1
#define WEIGHT_MIDPOINT 7
#define CROSSOVER_RATE 70 // percentage (e.g. * 100)
#define MUTATION_RATE 1 // percentage
#define PRUNE_RATE 1 // percentage
#define MUTATION_RANGE .2

using namespace std;

Chromosome::Chromosome() {
    m_fitness = 0.0;
    m_genes = array<double, CHROMOSOME_LEN>();
}

Chromosome::Chromosome(array<double, CHROMOSOME_LEN>& genes, double fitness) {
    m_genes = genes;
    m_fitness = fitness;
}

void Chromosome::setFitness(double fitness) {
    m_fitness = fitness;
}


double time_const_conversion(double x) {
    return 20 + 480 * x;
}

double bias_conversion(double x) {
    return -10 + 20 * x;
}

double synaptic_weight_conversion(double x) {
    return 10 * x;
}

void Chromosome::to_vector(std::vector<std::vector<double>>& vec) {
    // left side of network
    vector<double> neuron_params = vector<double>(PARAM_LEN);
    for (int j = 0; j < NEURONS_TO_SET_UP; j++) {
        for (int i = 0; i < PARAM_LEN; i++) {
            double param_val = m_genes[i + j * PARAM_LEN];
            if (i == 0) param_val = time_const_conversion(param_val);
            else if (i == 1) param_val = bias_conversion(param_val);
            else param_val = synaptic_weight_conversion(param_val);
            neuron_params[i] = param_val;
        }
        vec.push_back(neuron_params);
    }
    
    // right side of network
    for (int j = 0; j < NEURONS_TO_SET_UP; j++) {
        for (int i = 0; i < PARAM_LEN; i++) {
            int offset = 0;
            if (i > CONST_INDEX) {
                if (i < WEIGHT_MIDPOINT) offset = 5;
                else offset = -5;
            }
            double param_val = m_genes[offset + i + j * PARAM_LEN];
            if (i == 0) param_val = time_const_conversion(param_val);
            else if (i == 1) param_val = bias_conversion(param_val);
            else param_val = synaptic_weight_conversion(param_val);
            neuron_params[i] = param_val;
        }
        vec.push_back(neuron_params);
    }
}


void Chromosome::setGeneticAlgo(GeneticAlgo* ga) {
    m_ga = ga;
}

void Chromosome::decode() {
    for (int i = 0; i < NEURONS_TO_SET_UP; i++) {
        for (int j = 0; j < PARAM_LEN; j++) {
            double val = m_genes[i * PARAM_LEN + j];
            if (j == 0) val = time_const_conversion(val);
            else if (j == 1) val = bias_conversion(val);
            else val = synaptic_weight_conversion(val);
            cout << val << " ";
        }
        cout << "*" <<  endl;
    }
}

void Chromosome::getRandomBits() {
    for (int i = 0; i < CHROMOSOME_LEN; i++) {
        int x = rand() % 100;
        m_genes[i] = x / 100.0;
    }
}

void Chromosome::printBits() {
    for (auto& g : m_genes) {
        cout << g << " ";
    }
    cout << endl;
}



void Chromosome::crossover(array<double, CHROMOSOME_LEN>& child1, array<double, CHROMOSOME_LEN>& child2) {
    int crossChance = rand() % 100;
    if (crossChance < CROSSOVER_RATE) {
        int x = rand() % CHROMOSOME_LEN;
        float temp = 0.0f;
        for (int i = x; i < CHROMOSOME_LEN; i++) {
            temp = child1[i];
            child1[i] = child2[i];
            child2[i] = temp;
        }
    }
}


void Chromosome::prune(array<double, CHROMOSOME_LEN>& child) {
    for (int i = 0; i < CHROMOSOME_LEN; i++) {
        int x = rand() % 100 + 1;
        if (x <= PRUNE_RATE) {
            //cout << "pruned a gene" << endl;
            child[i] = 0.0;
        }
    }
}

void Chromosome::mutate(array<double, CHROMOSOME_LEN>& child) {
    for (int i = 0; i < CHROMOSOME_LEN; i++) {
        int x = rand() % 100 + 1;
        if (x <= MUTATION_RATE) {
            //cout << "mutated a gene" << endl;
            // new_value = old_value + r * rand
            // rand is between -.5 and .5
            // min max locks the final value betwixt 0.0 and 1.0
            child[i] = min(1.0, max(0.0, child[i] + MUTATION_RANGE * (x - 50.0)/100.0));
        }
        
    }
}

double Chromosome::getFitness() {
    return m_fitness;
}


array<double, CHROMOSOME_LEN>& Chromosome::rouletteSelect(double totalFitness, Chromosome c_arr[], int len) {
    // gets  number in range [0, totalFitness)
    double pointSelected = 0;
    if (((int)totalFitness) != 0)
        pointSelected = (rand() % ((int) (totalFitness * 100))) / 100.0;
    double fitnessAccum = 0.0;
    for (int i = 0; i < len; i++) {
        fitnessAccum += c_arr[i].getFitness();
        if (fitnessAccum >= pointSelected)
            return c_arr[i].m_genes;
    }
    cerr << "Error with roulette select, should not reach this." << endl;
    exit(EXIT_FAILURE);
}


