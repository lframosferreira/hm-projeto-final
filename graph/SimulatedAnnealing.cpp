#include "SimulatedAnnealing.h"
#include "graph.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>
#include <cmath>

SimulatedAnnealing::SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize)
    : initialTemperature(initialTemp), endTemperature(endTemp), coolingCoefficient(coolingCoeff), currentTemperature(initialTemp),
      graph(graph), m(cliqueSize), n(graph.get_number_of_vertices()), rng(std::random_device{}()) {
    initialize();
}

void SimulatedAnnealing::initialize() {
    permutation.resize(n);
    std::iota(permutation.begin(), permutation.end(), 1); // 1-based indexing
    adj_list_t adjacency_list = graph.get_adjacency_list();

    vertexDegrees.resize(n + 1); // 1-based indexing
    for (int i = 1; i <= n; ++i) {
        vertexDegrees[i] = adjacency_list[i].size();
    }
}

void SimulatedAnnealing::adjustPermutation() {
    std::sort(permutation.begin(), permutation.end(), [this](int a, int b) {
        return vertexDegrees[a] < vertexDegrees[b];
    });
}

std::pair<int, int> SimulatedAnnealing::selectVertices() {
    std::uniform_int_distribution<int> cliqueDist(0, m - 1);
    std::uniform_int_distribution<int> nonCliqueDist(m, n - 1);

    int attempts = 0;
    while (attempts < 8 * n) {
        int u = cliqueDist(rng);
        int w = nonCliqueDist(rng);

        double f0u = computePartialObjective(u);
        double f0w = computePartialObjective(w);

        if (f0u <= f0w || ++attempts >= 8 * n) {
            return {u, w};
        }
    }

    return {0, m}; // Default fallback
}

double SimulatedAnnealing::computeObjectiveFunction(const std::vector<int>& perm) {
    double score = 0;
    adj_list_t adjacency_list = graph.get_adjacency_list();
    for (int i = 0; i < m; ++i) {
        computePartialObjective(i);
    }
    return score;
}

double SimulatedAnnealing::computePartialObjective(int vertexIndex) {
    double score = 0;
    adj_list_t adjacency_list = graph.get_adjacency_list();
    for (int i = 0; i < m; ++i) {
        if (i != vertexIndex) {
            int u = permutation[vertexIndex];
            int v = permutation[i];
            if (std::find(adjacency_list[u].begin(), adjacency_list[u].end(), v) != adjacency_list[u].end()) {
                score += 1;
            }
        }
    }
    return score;
}

void SimulatedAnnealing::performStateTransition(int u, int w) {
    std::swap(permutation[u], permutation[w]);
}

bool SimulatedAnnealing::acceptNewState(double deltaF) {
    if (deltaF <= 0) {
        return true;
    }
    double probability = std::exp(-deltaF / currentTemperature);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng) < probability;
}

void SimulatedAnnealing::run() {
    adjustPermutation();
    double currentF = computeObjectiveFunction(permutation);

    while (currentTemperature > endTemperature) {
        auto [u, w] = selectVertices();
        performStateTransition(u, w);

        double newF = computeObjectiveFunction(permutation);
        double deltaF = newF - currentF;

        if (acceptNewState(deltaF)) {
            currentF = newF;
        } else {
            performStateTransition(u, w); // Revert the change
        }

        currentTemperature *= coolingCoefficient;

        if (currentF == 0) {
            break; // Solution found
        }
    }

    std::cout << "Final objective function value: " << currentF << std::endl;
}
