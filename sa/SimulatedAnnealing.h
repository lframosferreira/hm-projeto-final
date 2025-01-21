#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "../graph/graph.h"
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>
#include <cmath>
using namespace std;


class SimulatedAnnealing {
private:
    double initialTemperature;
    double endTemperature;
    double coolingCoefficient;
    double currentTemperature;
    double currentF;

    Graph graph;
    int m; // Size of the clique
    int n; // Number of vertices in the graph

    vector<int> permutation;
	vector<int> last_clique;
    vector<int> vertexDegrees;

    mt19937 rng;

    void initialize();
    void initialize(vector<int> perm);
    void setupDegrees();
    void adjustPermutation();
    pair<int, int> selectVertices();
    double computeObjectiveFunction(const vector<int>& perm);
    double computePartialObjective(int vertexIndex);
    void performStateTransition(int u, int w);
    bool acceptNewState(double deltaF);

public:
    SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize, std::vector<int> perm);

    SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize);

    std::vector<int> run();
    std::vector<int> maximum_clique();
};

#endif // SIMULATED_ANNEALING_H
