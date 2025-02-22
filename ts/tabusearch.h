#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "../graph/graph.h"
#include <vector>
#include <deque>
#include <cassert>

class TabuSearch {
private:
    const Graph &graph;
    std::vector<int> currentSolution;
    std::vector<int> bestSolution;
	int bestClique;
    int maxTabuSize;
    int maxIterations;

    std::vector<std::vector<int>> generateNeighborhood(const std::vector<int>& solution, const std::deque<std::vector<int>>& tabuList);
    std::vector<int> selectBestNeighbor(const std::vector<std::vector<int>>& neighborhood);
    int computeObjectiveFunction(const std::vector<int>& solution);
    bool isClique(const std::vector<int>& solution);

public:
    TabuSearch(const Graph &graph, const std::vector<int>& initialSolution, int maxTabuSize, int maxIterations);
    void run();
    std::vector<int> getBestSolution() const;
	int getBestClique() const;
};

#endif // TABU_SEARCH_H
