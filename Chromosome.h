#ifndef __central_pattern_generator__Chromosome__
#define __central_pattern_generator__Chromosome__

#include <iostream>
#include <string>
#include <array>
#include <vector>


#define PERFECT_FITNESS 9999.0
#define CHROMOSOME_LEN 48


class GeneticAlgo;
class Chromosome {
private:
    std::array<double, CHROMOSOME_LEN> m_genes;
    GeneticAlgo* m_ga;
    double m_fitness;
    
public:
    Chromosome();
    Chromosome(std::array<double, CHROMOSOME_LEN>&, double fitness=0.0);
    void setFitness(double);
    double getFitness();
    void getRandomBits();
    void printBits();
    void printDecoded();
    void setGeneticAlgo(GeneticAlgo* ga);
    void calcFitness();
    static void mutate(std::array<double, CHROMOSOME_LEN>& child);
    static void crossover(std::array<double, CHROMOSOME_LEN>& child1, std::array<double, CHROMOSOME_LEN>& child2);
    static std::array<double, CHROMOSOME_LEN>& rouletteSelect(double totalFitness, Chromosome c_arr[], Chromosome* cur_best, int len);
    void decode();
    void to_vector(std::vector<std::vector<double>>& vec);
    static void prune(std::array<double, CHROMOSOME_LEN>& child);

    
};


#endif /* defined(__central_pattern_generator__Chromosome__) */
