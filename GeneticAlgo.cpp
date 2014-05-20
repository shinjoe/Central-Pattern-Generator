#include "GeneticAlgo.h"
#include "Chromosome.h"
#include "CentralPatternGenerator.h"
#include <iostream>
#include <string>

#define GIVE_UP_THRESHOLD 15
#define POP_SIZE 50

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
    
    while (!done) {
        double totalFitness = 0.0;
        for (auto& c : c_arr) {
            //c.printBits();
            vector<vector<double>> vec = vector<vector<double>>();
            c.to_vector(vec);
            cpg.initNet(vec);
            cpg.run();
            double curFitness = cpg.calcFitness();
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
        
        if (done) break;
        
        array<Chromosome, POP_SIZE> nextGen;
        int newGenCount = 0;
        // keep going until we made POP_SIZE more children
        while (newGenCount < POP_SIZE) {
            // choose two parents to crossover
            array<float, CHROMOSOME_LEN> child1 = Chromosome::rouletteSelect(totalFitness, c_arr.data(), POP_SIZE);
            array<float, CHROMOSOME_LEN> child2 = Chromosome::rouletteSelect(totalFitness, c_arr.data(), POP_SIZE);
            
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
    mapping = map<string, string>();
    mapping["0000"] = "0";
    mapping["0001"] = "1";
    mapping["0010"] = "2";
    mapping["0011"] = "3";
    mapping["0100"] = "4";
    mapping["0101"] = "5";
    mapping["0110"] = "6";
    mapping["0111"] = "7";
    mapping["1000"] = "8";
    mapping["1001"] = "9";
    mapping["1010"] = "+";
    mapping["1011"] = "-";
    mapping["1100"] = "*";
    mapping["1101"] = "/";
    // null mappings for 1110 and 1111; unused
    mapping["1110"] = "";
    mapping["1111"] = "";
}