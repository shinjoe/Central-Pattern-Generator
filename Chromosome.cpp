#include "Chromosome.h"
#include "GeneticAlgo.h"
#include <iostream>
#include <cmath>

#define TARGET 20
#define GENE_LEN 4
#define CROSSOVER_RATE 70 // percentage (e.g. * 100)
#define MUTATION_RATE 1 // percentage
#define MUTATION_RANGE .2

using namespace std;

Chromosome::Chromosome() {
    m_genes = array<float, CHROMOSOME_LEN>();
    m_fitness = 0.0;
}

Chromosome::Chromosome(array<float, CHROMOSOME_LEN>& genes, double fitness) {
    m_genes = genes;
    m_fitness = fitness;
}

void Chromosome::setGeneticAlgo(GeneticAlgo* ga) {
    m_ga = ga;
}

void Chromosome::getRandomBits() {
    for (int i = 0; i < CHROMOSOME_LEN; i++) {
        int x = rand() % 2;
        m_genes[i] = x;
    }
}

void Chromosome::printBits() {
    for (auto& g : m_genes) {
        cout << g;
    }
    cout << endl;
}

bool isNumber(string curElem) {
    return curElem == "0" || curElem == "1" ||
    curElem == "2" || curElem == "3" ||
    curElem == "4" || curElem == "5" ||
    curElem == "6" || curElem == "7" ||
    curElem == "8" || curElem == "9";
}

bool isOp(string curElem) {
    return curElem == "+" || curElem == "-" ||
    curElem == "*" || curElem == "/";
}

void Chromosome::calcFitness() {
    bool expectingNumber = true;
    int index = 0;
    int accum = 0;
    string prevOp = "+";
    while (index < CHROMOSOME_LEN) {
        string curString = "";
        // TODO: CHANGE TO NOT (INT)
        for (int i = index; i < index + GENE_LEN; i++) curString += to_string((int)m_genes[i]);
        string curElem = m_ga->mapping[curString];
        if (curElem == "") {
            index += GENE_LEN;
            continue;
        }
        else if (expectingNumber) {
            if (isNumber(curElem)) {
                int curNum = stoi(curElem);
                if (prevOp == "+") accum += curNum;
                else if (prevOp == "-") accum -= curNum;
                else if (prevOp == "*") accum *= curNum;
                else if (prevOp == "/") {
                    // disallow division by zero
                    if (curNum == 0) {
                        cout << "skipping division by zero" << endl;
                        index += GENE_LEN;
                        continue;
                    }
                    accum /= curNum;
                }
                else cerr << "UNEXPECTED OP" << endl;
                expectingNumber = false;
            }
        } else if (!expectingNumber) {
            if (isOp(curElem)) {
                prevOp = curElem;
                expectingNumber = true;
            }
        }
        
        index += GENE_LEN;
    }
    
    // exit if we perfectly hit the target
    if (accum - TARGET == 0) {
        m_fitness = PERFECT_FITNESS;
        return;
    }
    m_fitness = 1.0 / abs(accum - TARGET);
    cout << "fitness: " << m_fitness << endl;
    
}



void Chromosome::crossover(array<float, CHROMOSOME_LEN>& child1, array<float, CHROMOSOME_LEN>& child2) {
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


void Chromosome::mutate(array<float, CHROMOSOME_LEN>& child) {
    for (int i = 0; i < CHROMOSOME_LEN; i++) {
        int x = rand() % 100 + 1;
        if (x <= MUTATION_RATE) {
            cout << "mutated a gene" << endl;
            // new_value = old_value + r * rand
            // rand is between -.5 and .5
            child[i] = child[i] + MUTATION_RANGE * (x - 50)/100.0;
        }
        
    }
}

double Chromosome::getFitness() {
    return m_fitness;
}


array<float, CHROMOSOME_LEN>& Chromosome::rouletteSelect(double totalFitness, Chromosome c_arr[], int len) {
    // gets  number in range [0, totalFitness)
    double pointSelected = (rand() % ((int) (totalFitness * 100))) / 100.0;
    double fitnessAccum = 0.0;
    for (int i = 0; i < len; i++) {
        fitnessAccum += c_arr[i].getFitness();
        if (fitnessAccum >= pointSelected)
            return c_arr[i].m_genes;
    }
    cerr << "Error with roulette select, should not reach this." << endl;
    exit(EXIT_FAILURE);
}


void Chromosome::decode() {
    string res = "";
    for (int i = 0; i < CHROMOSOME_LEN; i += GENE_LEN) {
        string cur = "";
        // TODO REMOVE (int) FOR FLOAT
        for (int j = i; j < i + GENE_LEN; j++) cur += to_string((int)m_genes[j]);
        res += m_ga->mapping[cur] + " ";
    }
    
    cout << res << endl;
}


