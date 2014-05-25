#include "GeneticAlgo.h"
#include "Chromosome.h"
#include "CentralPatternGenerator.h"
#include <iostream>
#include <string>

#define GIVE_UP_THRESHOLD 600
#define POP_SIZE 100

#include <array>

using namespace std;


void GeneticAlgo::run() {
    cout << "Running..." << endl;
    CentralPatternGenerator cpg;
    array<Chromosome, POP_SIZE> c_arr;
    int generationCount = 0;
    // initialize chromosomes with random genes
    for (auto& c : c_arr) {
        c.setGeneticAlgo(this);
        c.getRandomBits();
        c.printBits();
    }
    
    cout << "----------------------------------------" << endl;
    Chromosome* bestChromosome = nullptr;
    double bestFitness = 0.0;
    while (!done) {
        double totalFitness = 0.0;
        double cur_best_fitness = 0.0;
        bool first = true;
        Chromosome* current_gen_best_chromosome = nullptr;
        for (auto& c : c_arr) {
            //c.printBits();
            vector<vector<double>> vec = vector<vector<double>>();
            c.to_vector(vec);
            cpg.initNet(vec);
            cpg.run();
            double curFitness = cpg.calcFitness();
            if (first) {
                cur_best_fitness = curFitness;
                current_gen_best_chromosome = &c;
                first = false;
            } else {
                if (curFitness > cur_best_fitness) {
                    cur_best_fitness = curFitness;
                    current_gen_best_chromosome = &c;
                }
            }
            
            if (curFitness > bestFitness) {
                bestFitness = curFitness;
                bestChromosome = &c;
            }
            c.setFitness(curFitness);
            totalFitness += curFitness;
            cout << "fit " << curFitness << endl;
            
            /*if (curFitness == PERFECT_FITNESS) {
                cout << "Solution found in " << generationCount << " generations." << endl;
                c.printBits();
                //c.decode();
                done = true;
                break;
            }*/
        }
        cout << "************************************" << endl;
        cout << "best fitness of cur " << bestFitness << endl;
        bestChromosome->decode();
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
        
        if (done) break;
        
        array<Chromosome, POP_SIZE> nextGen;
        int newGenCount = 0;
        // keep going until we made POP_SIZE more children
        while (newGenCount < POP_SIZE) {
            // choose two parents to crossover
            array<double, CHROMOSOME_LEN> child1 = Chromosome::rouletteSelect(totalFitness, c_arr.data(), current_gen_best_chromosome, POP_SIZE);
            array<double, CHROMOSOME_LEN> child2 = Chromosome::rouletteSelect(totalFitness, c_arr.data(), current_gen_best_chromosome, POP_SIZE);
            
            Chromosome::crossover(child1, child2);
            Chromosome::mutate(child1);
            Chromosome::mutate(child2);
            Chromosome::prune(child1);
            Chromosome::prune(child2);
            
            nextGen[newGenCount++] = Chromosome(child1);
            nextGen[newGenCount++] = Chromosome(child2);
            nextGen[newGenCount-2].setGeneticAlgo(this);
            nextGen[newGenCount-1].setGeneticAlgo(this);
        }
        // the next generation becomes the new current generation in the next iteration
        c_arr = nextGen;
        
        generationCount++;
        
        if (generationCount > GIVE_UP_THRESHOLD) {
            cout << "No solutions, taking too many iterations..." << endl;
            done = true;
        }
        
    }
   /* vector<vector<double>> vec = vector<vector<double>>();
    c_arr[0].to_vector(vec);
    cpg.initNet(vec);
    cpg.run();
    cout <<  "final fitness " << cpg.calcFitness() << endl;
    c_arr[0].decode();*/
}

GeneticAlgo::GeneticAlgo() {
    done = false;
}