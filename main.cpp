#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "CentralPatternGenerator.h"
#include "GeneticAlgo.h"

int main(int argc, const char * argv[]) {
    srand((int) time(NULL));
    
    CentralPatternGenerator cpg;
    std::vector<std::vector<double>> vec = std::vector<std::vector<double>>();
    cpg.initNet(vec);
    cpg.run();
    
    //GeneticAlgo g;
    //g.run();
    
    return EXIT_SUCCESS;
}

