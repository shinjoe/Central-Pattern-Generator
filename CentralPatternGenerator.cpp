#include "CentralPatternGenerator.h"

using namespace std;

#define TIMESTEP .1
#define TRIAL_LEN 1 // seconds
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP

CentralPatternGenerator::CentralPatternGenerator() {
    //                           tau  bias  Ml   Al   Bl     Cl  BSl  Mr   Ar    Br   Cr   BSr
    vector<double> m_left_init  { 20,  0.2, 0,    0,  -8.2, -10,   5, 0,    0,    0, -4.1, 0};
    vector<double> a_left_init  {297,  2.9, 0,    0,     0,   0,   0, 0, -4.4, -3.4, -2.8, 0};
    vector<double> b_left_init  { 57, -6.4, 0, -2.1,  -5.8,   0, 8.2, 0,    0, -8.8, -9.6, 0};
    vector<double> c_left_init  { 20,  5.6, 0, -2.2,  -9.7,  -4, 2.9, 0,    0,    0, -9.9, 0};
    vector<double> bs_init {};
    M_left  = Neuron(m_left_init, -.605, .6, MOTO,      "M_left  ");
    A_left  = Neuron(a_left_init, -2.05, .3, INTER,     "A_left  ");
    B_left  = Neuron(b_left_init,  7.25, .3, INTER,     "B_left  ");
    C_left  = Neuron(c_left_init, -7.79, .9, INTER,     "C_left  ");
    BS_left = Neuron(bs_init,         1,  1, BRAINSTEM, "BS_left ");
    
    // right side is symmetric
    vector<double> m_right_init  { 20, 0, 0,    0,    0, -4.1, 0, 0,    0, -8.2, -10,   5};
    vector<double> a_right_init  {297, 0, 0, -4.4, -3.4, -2.8, 0, 0,    0,    0,   0,   0};
    vector<double> b_right_init  { 57, 0, 0,    0, -8.8, -9.6, 0, 0, -2.1, -5.8,   0, 8.2};
    vector<double> c_right_init  { 20, 0, 0,    0,    0, -9.9, 0, 0, -2.2, -9.7,  -4, 2.9};
    vector<double> bs_right_init {};
    
    M_right  = Neuron(m_right_init,  4.39, .01,  MOTO,    "M_right ");
    A_right  = Neuron(a_right_init, -2.15, .32, INTER,     "A_right ");
    B_right  = Neuron(b_right_init,  7.25,  .3, INTER,     "B_right ");
    C_right  = Neuron(c_right_init, -7.79,  .9, INTER,     "C_right ");
    BS_right = Neuron(bs_init,          1,   1, BRAINSTEM, "BS_right");
    
    m_network = vector<Neuron *>();
    m_network.push_back(&M_left);
    m_network.push_back(&A_left);
    m_network.push_back(&B_left);
    m_network.push_back(&C_left);
    m_network.push_back(&BS_left);
    m_network.push_back(&M_right);
    m_network.push_back(&A_right);
    m_network.push_back(&B_right);
    m_network.push_back(&C_right);
    m_network.push_back(&BS_right);
    
    m_solver = RungeKutta(m_network);

}


void CentralPatternGenerator::run() {
    double time = 0.0;
    for (int curTick = 0; curTick < 1500; curTick++) {
        for (auto& n : m_network) {
            m_solver.calcMeanMembranePotential(*n, time, TIMESTEP);
            m_solver.calcFiringFrequency(*n);
            //cout << n.getName() << "  M: " << n.getM() << "  X: " << n.getX() << endl;
            if (n->getName() == "M_left  ")
                cout << time << "\t" << n->getX() << endl;
        }
             time += TIMESTEP;
    }
}