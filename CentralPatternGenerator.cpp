#include "CentralPatternGenerator.h"
#include <fstream>
#include "math.h"

using namespace std;

#define TIMESTEP 1
#define TRIAL_LEN 1 // seconds
#define DUTY_CYCLE .091 //  1/11, because we have 11 segments
#define TRIAL_TICKS TRIAL_LEN/TIMESTEP
//#define NEIGHBOR_WIDTH 5
#define NEIGHBOR_WIDTH 3

CentralPatternGenerator::CentralPatternGenerator() {
    m_last_few_points = array<double, CAPTURE_SIZE>();
    m_cur_chromosome = nullptr;
}

void CentralPatternGenerator::set_cur_chromosome(Chromosome * c) {
    m_cur_chromosome = c;
}

Chromosome * CentralPatternGenerator::get_cur_chromosome() {
    return m_cur_chromosome;
}

double CentralPatternGenerator::findXOf(int index) {
    return m_network[index].getX();
}

std::vector<Neuron> * CentralPatternGenerator::getNetwork() {
    return &m_network;
}

void CentralPatternGenerator::initNet(vector<vector<double>>& vec, vector<pair<int, int>>&  pair_vec,
                                      array<CentralPatternGenerator, 11> * cpg_arr, int cpg_index) {
    
    
    m_cpg_index = cpg_index;
    m_cpg_arr = cpg_arr;
    
      //                           tau  bias  Ml   Al   Bl     Cl  BSl  Mr   Ar    Br   Cr   BSr
    vector<double> m_left_init  { 20, -10, 0, 5.46, 0, 0,    .24,    0, 0,    5.78, 4.54, 1.4};
    vector<double> a_left_init  { 20, -10, 2.3, 3.32, 0, 4.08,    0,    0, 0,    0, 0, 3.96};
    vector<double> b_left_init  { 20, -10, 0,    0, 0, 6.72,    0,    0, 1.26, 7.28, 0, 3.08};
    vector<double> c_left_init  { 20, -10, 0,    0, 0, 0, 6.88, 2.24, 0,   3.02, 0, 0};
    
    vector<double> bs_init {};
    
    // vec[0]
    M_left  = Neuron(m_left_init, -.605, .6, MOTO,      "M_left  ", 0);
    A_left  = Neuron(a_left_init, -2.05, .3, INTER,     "A_left  ", 1);
    B_left  = Neuron(b_left_init,  7.25, .3, INTER,     "B_left  ", 2);
    C_left  = Neuron(c_left_init, -7.79, .9, INTER,     "C_left  ", 3);
    
    BS_left = Neuron(bs_init,    2,  1, BRAINSTEM, "BS_left ", 4);
    
    // right side is symmetric
    vector<double> m_right_init  { 20, -10, 0, 0,    5.78, 4.54, 1.4, 0, 5.46, 0, 0,    .24};
    vector<double> a_right_init  { 20, -10, 0, 0,    0, 0, 3.96, 2.3, 3.32, 0, 4.08,    0};
    vector<double> b_right_init  { 20, -10, 0, 1.26, 7.28, 0, 3.08, 0,    0, 0, 6.72,    0};
    vector<double> c_right_init  { 20, -10, 2.24, 0,   3.02, 0, 0, 0,    0, 0, 0, 6.88};
    
    // vec[4]
    M_right  = Neuron(m_right_init, 4.39,  .01,  MOTO,     "M_right ", 5);
    A_right  = Neuron(a_right_init, -2.15, .32, INTER,     "A_right ", 6);
    B_right  = Neuron(b_right_init,  7.25,  .3, INTER,     "B_right ", 7);
    C_right  = Neuron(c_right_init, -7.79,  .9, INTER,     "C_right ", 8);
    BS_right = Neuron(bs_init,    2,   1, BRAINSTEM, "BS_right", 9);
    
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
    
    m_solver = RungeKutta(&m_network, pair_vec, cpg_index, cpg_arr);
    
    m_using_network_one = true;

}

double absVal(double x) {
    return x < 0 ? -x : x;
}

int CentralPatternGenerator::findPeakIndex(int starting_index) {
    int index = -1;
    for (int i = starting_index; i > NEIGHBOR_WIDTH; i--) {
        if ((m_last_few_points[i] > m_last_few_points[i+NEIGHBOR_WIDTH]) && (m_last_few_points[i] > m_last_few_points[i-NEIGHBOR_WIDTH]))
            return i;
    }
    return index;
}

int CentralPatternGenerator::findTroughIndex(int i) {
    for (; i > 0; i--) {
        if ((m_last_few_points[i] < m_last_few_points[i+NEIGHBOR_WIDTH]) && (m_last_few_points[i] < m_last_few_points[i-NEIGHBOR_WIDTH]))
            return i;
    }
    return -1;
}

double CentralPatternGenerator::calcIntersegmentalFitness(CentralPatternGenerator& nextCpg) {
    int rightmost_peak_index = findPeakIndex(CAPTURE_SIZE - NEIGHBOR_WIDTH);
    if (rightmost_peak_index == -1) return 0;
    int penultimate_right_peak_index = findPeakIndex(rightmost_peak_index - NEIGHBOR_WIDTH);
    if (penultimate_right_peak_index == -1) return 0;
    double lambda = rightmost_peak_index - penultimate_right_peak_index;
    int rightmost_peak_second_cpg_index = nextCpg.findPeakIndex(CAPTURE_SIZE - NEIGHBOR_WIDTH);
    if (rightmost_peak_second_cpg_index == -1) return 0;
    double delta_t = absVal(rightmost_peak_second_cpg_index - rightmost_peak_index);
    //cout << "delta t " <<  delta_t << endl;
    //cout << "lambda " << lambda << endl;
    //if (delta_t == 0) return 0;
    int trough = findTroughIndex(CAPTURE_SIZE - NEIGHBOR_WIDTH);

    //cout << "duty " << (0.01 / (absVal(delta_t / lambda - DUTY_CYCLE))) << endl;
    cout << "d/l " <<  delta_t / lambda << endl;
    cout << "amp  " << absVal((m_last_few_points[rightmost_peak_index] - m_last_few_points[trough])) << endl;
    if (delta_t >= lambda/2 && delta_t < 0) return 0.0;
    return (0.02 / (absVal( delta_t / lambda - DUTY_CYCLE) + 0.02)) * absVal((m_last_few_points[rightmost_peak_index] - m_last_few_points[trough]));
    
}

double CentralPatternGenerator::calcFitness() {
    double sum_of_deltas = 0.0;
    for (int i = 1; i < CAPTURE_SIZE; i++) {
        sum_of_deltas += absVal(m_last_few_points[i] - m_last_few_points[i-1]);
    }
    return isnan(sum_of_deltas) ? 0 : sum_of_deltas;
}


void CentralPatternGenerator::run() {
    ofstream outfile0;
    ofstream outfile1;
    ofstream title;
    if (m_cpg_index == 0) {
        outfile0.open("out0.txt");
        title.open("genes.txt");
        title << get_cur_chromosome()->decode_interseg() << endl;
    }
    if (m_cpg_index == 1)
        outfile1.open("out1.txt");
    double time = 0.0;
    int maxTicks = 1000;
    int pointIndex = 0;
    for (int curTick = 0; curTick < maxTicks; curTick++) {
        m_cur  = m_using_network_one ? &m_network : &m_copy;
        m_prev = m_using_network_one ? &m_copy    : &m_network;
        m_solver.updateNetwork(m_prev);
        for (int i = 0; i < m_cur->size(); i++) {
            m_solver.calcMeanMembranePotential((*m_prev)[i], (*m_cur)[i], time, TIMESTEP);
            m_solver.calcFiringFrequency((*m_cur)[i]);
        }
        if (m_cpg_index == 0)
            outfile0 << time << "\t" << (*m_cur)[0].getX() << endl;
        if (m_cpg_index == 1)
            outfile1 << time << "\t" << /*(*m_cpg_arr)[3].findXOf(0)*/ (*m_cur)[0].getX() << endl;
        if (curTick >= maxTicks - CAPTURE_SIZE)
            m_last_few_points[pointIndex++] = (*m_cur)[0].getX();
        time += TIMESTEP;
        m_using_network_one = !m_using_network_one;
    }
    if (m_cpg_index == 0) {
        outfile0.close();
        title.close();
    }

    if (m_cpg_index == 1)
        outfile1.close();
}