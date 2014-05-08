#ifndef __central_pattern_generator__GeneticAlgo__
#define __central_pattern_generator__GeneticAlgo__


#include <map>

class GeneticAlgo {
public:
    GeneticAlgo();
    void run();
    std::map<std::string, std::string> mapping;
    bool done;
};


#endif /* defined(__central_pattern_generator__GeneticAlgo__) */
