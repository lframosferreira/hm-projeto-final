#include "tabusearch.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

TabuSearch::TabuSearch(Graph graph, std::vector<int> initialSolution, int maxTabuSize, int maxIterations)
    : graph(graph), currentSolution(initialSolution), maxTabuSize(maxTabuSize), maxIterations(maxIterations) {
    bestSolution = initialSolution;
}

std::vector<int> TabuSearch::run() {
    tabuList.clear();
    int iteration = 0;

    while (iteration < maxIterations) {
        std::vector<std::vector<int>> neighborhood = generateNeighborhood(currentSolution);

        std::vector<int> bestCandidate;
        double bestFitness = -1;

        for (const auto& candidate : neighborhood) {
            if (std::find(tabuList.begin(), tabuList.end(), candidate) == tabuList.end()) {
                double fitness = evaluateSolution(candidate);
                if (fitness > bestFitness) {
                    bestFitness = fitness;
                    bestCandidate = candidate;
                }
            }
        }

        currentSolution = bestCandidate;

        if (evaluateSolution(currentSolution) > evaluateSolution(bestSolution)) {
            bestSolution = currentSolution;
        }

        tabuList.push_back(currentSolution);
        if (tabuList.size() > maxTabuSize) {
            tabuList.pop_front();
        }

        iteration++;
    }

    return bestSolution;
}

std::vector<std::vector<int>> TabuSearch::generateNeighborhood(const std::vector<int>& solution) {
    std::vector<std::vector<int>> neighborhood;

    for (size_t i = 0; i < solution.size(); ++i) {
        for (size_t j = i + 1; j < solution.size(); ++j) {
            std::vector<int> neighbor = solution;
            std::swap(neighbor[i], neighbor[j]);
            neighborhood.push_back(neighbor);
        }
    }

    return neighborhood;
}

double TabuSearch::evaluateSolution(const std::vector<int>& solution) {
    double score = 0;
    const auto& adjList = graph.get_adjacency_list();

    for (size_t i = 0; i < solution.size(); ++i) {
        for (size_t j = i + 1; j < solution.size(); ++j) {
            if (std::find(adjList[solution[i]].begin(), adjList[solution[i]].end(), solution[j]) != adjList[solution[i]].end()) {
                score++;
            }
        }
    }

    return score;
}
