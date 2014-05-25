#include "CentralPatternGenerator.h"
#include <fstream>
#include "math.h"

using namespace std;

#define TIMESTEP 1
#define TRIAL_LEN 1 // seconds
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP

CentralPatternGenerator::CentralPatternGenerator() {
    m_last_few_points = array<double, CAPTURE_SIZE>();
}

void CentralPatternGenerator::initNet(vector<vector<double>>& vec) {
      //                           tau  bias  Ml   Al   Bl     Cl  BSl  Mr   Ar    Br   Cr   BSr
    /*vector<double> m_left_init  { 20, -10, 0, 5.46, 0, 0,    .24,    0, 0,    5.78, 4.54, 1.4};
    vector<double> a_left_init  { 20, -10, 2.3, 3.32, 0, 4.08,    0,    0, 0,    0, 0, 3.96};
    vector<double> b_left_init  { 20, -10, 0,    0, 0, 6.72,    0,    0, 1.26, 7.28, 0, 3.08};
    vector<double> c_left_init  { 20, -10, 0,    0, 0, 0, 6.88, 2.24, 0,   3.02, 0, 0};*/
    
    vector<double> bs_init {};
    
    // vec[0]
    M_left  = Neuron(vec[0], -.605, .6, MOTO,      "M_left  ");
    A_left  = Neuron(vec[1], -2.05, .3, INTER,     "A_left  ");
    B_left  = Neuron(vec[2],  7.25, .3, INTER,     "B_left  ");
    C_left  = Neuron(vec[3], -7.79, .9, INTER,     "C_left  ");
    
    BS_left = Neuron(bs_init,    2,  1, BRAINSTEM, "BS_left ");
    
    // right side is symmetric
    /*vector<double> m_right_init  { 20, -10, 0, 0,    5.78, 4.54, 1.4, 0, 5.46, 0, 0,    .24};
    vector<double> a_right_init  { 20, -10, 0, 0,    0, 0, 3.96, 2.3, 3.32, 0, 4.08,    0};
    vector<double> b_right_init  { 20, -10, 0, 1.26, 7.28, 0, 3.08, 0,    0, 0, 6.72,    0};
    vector<double> c_right_init  { 20, -10, 2.24, 0,   3.02, 0, 0, 0,    0, 0, 0, 6.88};*/
    
    // vec[4]
    M_right  = Neuron(vec[4], 4.39,  .01,  MOTO,    "M_right ");
    A_right  = Neuron(vec[5], -2.15, .32, INTER,     "A_right ");
    B_right  = Neuron(vec[6],  7.25, .3, INTER,     "B_right ");
    C_right  = Neuron(vec[7], -7.79, .9, INTER,     "C_right ");
    BS_right = Neuron(bs_init,    2,   1, BRAINSTEM, "BS_right");
    
    m_network = vector<Neuron>();
    m_network.push_back(M_left);
    m_network.push_back(A_left);
    m_network.push_back(B_left);
    m_network.push_back(C_left);
    m_network.push_back(BS_left);
    m_network.push_back(M_right);
    m_network.push_back(A_right);
    m_network.push_back(B_right);
    m_network.push_back(C_right);
    m_network.push_back(BS_right);
    
    m_copy = vector<Neuron>();
    m_copy.push_back(M_left);
    m_copy.push_back(A_left);
    m_copy.push_back(B_left);
    m_copy.push_back(C_left);
    m_copy.push_back(BS_left);
    m_copy.push_back(M_right);
    m_copy.push_back(A_right);
    m_copy.push_back(B_right);
    m_copy.push_back(C_right);
    m_copy.push_back(BS_right);
    
    m_solver = RungeKutta(&m_network);
    
    m_using_network_one = true;

}

double absVal(double x) {
    return x < 0 ? -x : x;
}

double CentralPatternGenerator::calcFitness() {
    double sum_of_deltas = 0.0;
    for (int i = 1; i < CAPTURE_SIZE; i++) {
        sum_of_deltas += absVal(m_last_few_points[i] - m_last_few_points[i-1]);
    }
    return isnan(sum_of_deltas) ? 0 : sum_of_deltas;
}


void CentralPatternGenerator::run() {
    ofstream outfile;
    outfile.open("out.txt");
    double time = 0.0;
    int maxTicks = 5000;
    int pointIndex = 0;
    for (int curTick = 0; curTick < maxTicks; curTick++) {
        m_cur  = m_using_network_one ? &m_network : &m_copy;
        m_prev = m_using_network_one ? &m_copy    : &m_network;
        m_solver.updateNetwork(m_prev);
        for (int i = 0; i < m_cur->size(); i++) {
            m_solver.calcMeanMembranePotential((*m_prev)[i], (*m_cur)[i], time, TIMESTEP);
            m_solver.calcFiringFrequency((*m_cur)[i]);
        }
        outfile << time << "\t" << m_network[0].getX() << endl;
        //cout << m_network[0].getM() << endl;
        if (curTick >= maxTicks - CAPTURE_SIZE)
            m_last_few_points[pointIndex++] = m_network[0].getX();
        time += TIMESTEP;
        m_using_network_one = !m_using_network_one;
    }
    outfile.close();
}