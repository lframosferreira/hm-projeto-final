#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "graph.h"
#include <deque>
#include <vector>

class TabuSearch {
private:
    Graph graph;
    std::vector<int> currentSolution;
    std::vector<int> bestSolution;
    std::deque<std::vector<int>> tabuList;
    int maxTabuSize;
    int maxIterations;

    std::vector<std::vector<int>> generateNeighborhood(const std::vector<int>& solution);
    double evaluateSolution(const std::vector<int>& solution);

public:
    TabuSearch(Graph graph, std::vector<int> initialSolution, int maxTabuSize, int maxIterations);
    std::vector<int> run();
};

#endif // TABU_SEARCH_H
