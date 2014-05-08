#include "CentralPatternGenerator.h"

using namespace std;

#define TIMESTEP .01
#define TRIAL_LEN 1 // seconds
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP

CentralPatternGenerator::CentralPatternGenerator() {
    vector<double> m_left_init  { 20,  0.2, 0,    0,  -8.2, -10,   0, 0,    0,    0, -4.1, 0};
    vector<double> a_left_init  {297,  2.9, 0,    0,     0,   0,   0, 0, -4.4, -3.4, -2.8, 0};
    vector<double> b_left_init  { 57, -6.4, 0, -2.1,  -5.8,   0, 8.2, 0,    0, -8.8, -9.6, 0};
    vector<double> c_left_init  { 20,  5.6, 0, -2.2,  -9.7,  -4, 2.9, 0,    0,    0, -9.9, 0};
    vector<double> bs_init {};
    M_left  = Neuron(m_left_init,  1, 1, MOTO);
    A_left  = Neuron(a_left_init,  1, 1, INTER);
    B_left  = Neuron(b_left_init,  1, 1, INTER);
    C_left  = Neuron(c_left_init,  1, 1, INTER);
    BS_left = Neuron(bs_init,      1, 1, BRAINSTEM);
    
    // right side is symmetric
    vector<double> m_right_init  { 20,   .2, 0,    0,    0, -4.1, 0, 0,    0, -8.2, -10,   5};
    vector<double> a_right_init  {297,  2.9, 0, -4.4, -3.4, -2.8, 0, 0,    0,    0,   0,   0};
    vector<double> b_right_init  { 57, -6.4, 0,    0, -8.8, -9.6, 0, 0, -2.1, -5.8,   0, 8.2};
    vector<double> c_right_init  { 20,  5.6, 0,    0,    0, -9.9, 0, 0, -2.2, -9.7,  -4, 2.9};
    vector<double> bs_right_init {};
    
    M_right  = Neuron(m_right_init,  1, 1, MOTO);
    A_right  = Neuron(a_right_init,  1, 1, INTER);
    B_right  = Neuron(b_right_init,  1, 1, INTER);
    C_right  = Neuron(c_right_init,  1, 1, INTER);
    BS_right = Neuron(bs_init,       1, 1, BRAINSTEM);
}


void CentralPatternGenerator::run() {
    for (int curTick = 0; curTick < TRIAL_TICKS; curTick++) {
        cout << "Running tick "  << curTick << endl;
    }
}