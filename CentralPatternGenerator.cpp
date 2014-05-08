#include "CentralPatternGenerator.h"

using namespace std;

#define TIMESTEP .01
#define TRIAL_LEN 1 // seconds
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP

void CentralPatternGenerator::run() {
    for (int curTick = 0; curTick < TRIAL_TICKS; curTick++) {
        cout << "Running tick "  << curTick << endl;
    }
}