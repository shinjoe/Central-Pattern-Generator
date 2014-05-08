#ifndef __central_pattern_generator__Chromosome__
#define __central_pattern_generator__Chromosome__

#include <iostream>
#include <string>


#define CHROMOSOME_LEN 40
#define PERFECT_FITNESS 9999.0
#include <string>

class GeneticAlgo;
class Chromosome {
private:
    std::string m_bits;
    double m_fitness;
    GeneticAlgo* m_ga;
    
public:
    Chromosome();
    Chromosome(std::string& bits, double fitness=0.0);
    void getRandomBits();
    void printBits();
    void printDecoded();
    void setGeneticAlgo(GeneticAlgo* ga);
    void calcFitness();
    static void mutate(std::string& child);
    static void crossover(std::string& child1, std::string& child2);
    static std::string rouletteSelect(double totalFitness, Chromosome c_arr[], int len);
    double getFitness();
    void decode();
    
};


#endif /* defined(__central_pattern_generator__Chromosome__) */
