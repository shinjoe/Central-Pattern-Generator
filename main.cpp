#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "CentralPatternGenerator.h"
#include "GeneticAlgo.h"

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    
    GeneticAlgo g;
    g.run();
    
    //CentralPatternGenerator cpg;
    //cpg.run();
    return EXIT_SUCCESS;
}

