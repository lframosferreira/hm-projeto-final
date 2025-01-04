#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "graph.h"
#include <vector>
#include <random>

class SimulatedAnnealing {
private:
    double initialTemperature;
    double endTemperature;
    double coolingCoefficient;
    double currentTemperature;

    Graph graph;
    int m; // Size of the clique
    int n; // Number of vertices in the graph

    std::vector<int> permutation;
    std::vector<int> vertexDegrees;

    std::mt19937 rng;

    void initialize();
    void adjustPermutation();
    std::pair<int, int> selectVertices();
    double computeObjectiveFunction(const std::vector<int>& perm);
    double computePartialObjective(int vertexIndex);
    void performStateTransition(int u, int w);
    bool acceptNewState(double deltaF);

public:
    SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize);
    void run();
};

#endif // SIMULATED_ANNEALING_H
