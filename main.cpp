#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "CentralPatternGenerator.h"
#include "GeneticAlgo.h"

int main(int argc, const char * argv[]) {
    srand((int) time(NULL));
    
    //CentralPatternGenerator cpg;
    //cpg.run();
    
    GeneticAlgo g;
    g.run();
    
    return EXIT_SUCCESS;
}

