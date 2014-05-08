#ifndef __central_pattern_generator__Chromosome__
#define __central_pattern_generator__Chromosome__

#include <iostream>
#include <string>
#include <array>
#include <vector>


#define CHROMOSOME_LEN 40
#define PERFECT_FITNESS 9999.0

class GeneticAlgo;
class Chromosome {
private:
    std::array<float, CHROMOSOME_LEN> m_genes;
    double m_fitness;
    GeneticAlgo* m_ga;
    
public:
    Chromosome();
    Chromosome(std::array<float, CHROMOSOME_LEN>&, double fitness=0.0);
    void getRandomBits();
    void printBits();
    void printDecoded();
    void setGeneticAlgo(GeneticAlgo* ga);
    void calcFitness();
    static void mutate(std::array<float, CHROMOSOME_LEN>& child);
    static void crossover(std::array<float, CHROMOSOME_LEN>& child1, std::array<float, CHROMOSOME_LEN>& child2);
    static std::array<float, CHROMOSOME_LEN>& rouletteSelect(double totalFitness, Chromosome c_arr[], int len);
    double getFitness();
    void decode();
    
};


#endif /* defined(__central_pattern_generator__Chromosome__) */
