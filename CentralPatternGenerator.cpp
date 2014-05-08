#include "CentralPatternGenerator.h"

using namespace std;

#define TIMESTEP .05

void CentralPatternGenerator::run(int ticks) {
    for (int curTick = 0; curTick < ticks; curTick++) {
        cout << "Running tick "  << curTick << endl;
    }
}