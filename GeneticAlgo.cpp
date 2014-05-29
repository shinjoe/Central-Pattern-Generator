#include "GeneticAlgo.h"
#include "Chromosome.h"
#include "CentralPatternGenerator.h"
#include <iostream>
#include <string>

#define GIVE_UP_THRESHOLD 200
#define POP_SIZE 100
#define NUM_SEGMENTS 11

#include <array>

using namespace std;

void init_syn_weights(vector<pair<int, int>>& vec, Chromosome& c);


void GeneticAlgo::run() {
    cout << "Running..." << endl;
    array<CentralPatternGenerator, NUM_SEGMENTS>  cpg_arr;
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
        
        
        for (int i = 0; i < cpg_arr.size(); i++) {
            vector<pair<int, int>> synaptic_spreading_weights;
            init_syn_weights(synaptic_spreading_weights, c_arr[i]);

            vector<vector<double>> vec = vector<vector<double>>();
            cpg_arr[i].initNet(vec, synaptic_spreading_weights, &cpg_arr, i);
        }
        
        for (int k = 0; k < cpg_arr.size(); k++) {
            cpg_arr[k].run();
        }
        
        double totalFitness = 0.0;
        double cur_best_fitness = 0.0;
        bool first = true;
        Chromosome* current_gen_best_chromosome = nullptr;
        for (int j = 0; j < cpg_arr.size() - 1; j++) {
            double curFitness = cpg_arr[j].calcIntersegmentalFitness(cpg_arr[j+1]);
            if (first) {
                cur_best_fitness = curFitness;
                current_gen_best_chromosome = &c_arr[j];
                first = false;
            } else {
                if (curFitness > cur_best_fitness) {
                    cur_best_fitness = curFitness;
                    current_gen_best_chromosome = &c_arr[j];
                }
            }
            
            if (curFitness > bestFitness) {
                bestFitness = curFitness;
                bestChromosome = &c_arr[j];
            }
            c_arr[j].setFitness(curFitness);
            totalFitness += curFitness;
            cout << "fit " << curFitness << endl;
        }
        cout << "************************************" << endl;
        cout << "best fitness of cur " << bestFitness << endl;
        //bestChromosome->decode();
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
   
}

GeneticAlgo::GeneticAlgo() {
    done = false;
}

void init_syn_weights(vector<pair<int, int>>& vec, Chromosome& c) {
    pair<int, int> ml_al = c.getSynWeightPair(0);
    pair<int, int> ml_br = c.getSynWeightPair(8);
    pair<int, int> ml_cr = c.getSynWeightPair(10);
    pair<int, int> al_al = c.getSynWeightPair(12);
    pair<int, int> al_cl = c.getSynWeightPair(16);
    pair<int, int> bl_cl = c.getSynWeightPair(28);
    pair<int, int> bl_ar = c.getSynWeightPair(30);
    pair<int, int> bl_br = c.getSynWeightPair(32);
    pair<int, int> cl_br = c.getSynWeightPair(44);
    pair<int, int> blank(0, 0);
    
    /* left side */
    // ml row
    vec.push_back(ml_al);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(ml_br);
    vec.push_back(ml_cr);
    // al row
    vec.push_back(al_al);
    vec.push_back(blank);
    vec.push_back(al_cl);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    // bl row
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(bl_cl);
    vec.push_back(bl_ar);
    vec.push_back(bl_br);
    vec.push_back(blank);
    // cl row
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(cl_br);
    vec.push_back(blank);
    
    /* right side */
    // mr row
    vec.push_back(blank);
    vec.push_back(ml_br);
    vec.push_back(ml_cr);
    vec.push_back(ml_al);
    vec.push_back(blank);
    vec.push_back(blank);
    
    // ar row
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(al_al);
    vec.push_back(blank);
    vec.push_back(al_cl);
    
    // br row
    vec.push_back(bl_ar);
    vec.push_back(bl_br);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(bl_cl);
    
    // cr row
    vec.push_back(blank);
    vec.push_back(cl_br);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    vec.push_back(blank);
    
    
}
