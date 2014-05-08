#include "CentralPatternGenerator.h"

using namespace std;

#define TIMESTEP .01
#define TRIAL_LEN 1 // seconds
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP

CentralPatternGenerator::CentralPatternGenerator() {
    vector<double> a_left_init {297.0, 2.9, 0, 0, -2.1, -2.2, 0, -4.4, 0, 0};
    A_left = Neuron(a_left_init);
    vector<double> b_left_init {57, -6.4, -8.2, 0, -5.8, -9.7, 0, -3.4, -8.8, 0};
}


void CentralPatternGenerator::run() {
    for (int curTick = 0; curTick < TRIAL_TICKS; curTick++) {
        cout << "Running tick "  << curTick << endl;
    }
}