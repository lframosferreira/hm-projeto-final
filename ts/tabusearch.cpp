#include "tabuSearch.h"
#include "../graph/graph.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <deque>

TabuSearch::TabuSearch(Graph graph, const std::vector<int> &initialSolution, int maxTabuSize, int maxIterations)
    : graph(graph), currentSolution(initialSolution), bestSolution(initialSolution),
      maxTabuSize(maxTabuSize), maxIterations(maxIterations) {}

void TabuSearch::run()
{
    std::deque<std::vector<int>> tabuList;
    int currentIteration = 0;
    int bestFitness = computeObjectiveFunction(bestSolution);

    while (currentIteration < maxIterations)
    {
        std::cout << "Iteration: " << currentIteration << std::endl;
        std::vector<std::vector<int>> neighborhood = generateNeighborhood(currentSolution, tabuList);

        if (neighborhood.empty())
        {
            std::cout << "No neighbors." << std::endl;
            break;
        }

        std::vector<int> bestNeighbor = selectBestNeighbor(neighborhood);
        int neighborFitness = computeObjectiveFunction(bestNeighbor);

        currentSolution = bestNeighbor;

        if (neighborFitness > bestFitness)
        {
            bestFitness = neighborFitness;
            bestSolution = bestNeighbor;
        }

        tabuList.push_back(currentSolution);
        if (tabuList.size() > maxTabuSize)
        {
            tabuList.pop_front();
        }

        currentIteration++;
    }

    std::cout << "Melhor aptidão encontrada: " << bestFitness << std::endl;
}

std::vector<std::vector<int>> TabuSearch::generateNeighborhood(
    const std::vector<int> &solution,
    const std::deque<std::vector<int>> &tabuList)
{
    std::vector<std::vector<int>> neighbors;
    adj_list_t adjacencyList = graph.get_adjacency_list();

    for (int i = 0; i < solution.size(); ++i)
    {
        for (int neighbor : adjacencyList[solution[i]])
        {
            std::vector<int> newSolution = solution;

            newSolution[i] = neighbor;

            if (std::find(tabuList.begin(), tabuList.end(), newSolution) == tabuList.end() &&
                isClique(newSolution))
            {
                neighbors.push_back(newSolution);
            }
        }
    }

    return neighbors;
}

std::vector<int> TabuSearch::selectBestNeighbor(const std::vector<std::vector<int>> &neighborhood)
{
    std::vector<int> bestNeighbor = neighborhood.front();
    int bestFitness = computeObjectiveFunction(bestNeighbor);

    for (const auto &neighbor : neighborhood)
    {
        int fitness = computeObjectiveFunction(neighbor);
        if (fitness > bestFitness)
        {
            bestFitness = fitness;
            bestNeighbor = neighbor;
        }
    }

    return bestNeighbor;
}

int TabuSearch::computeObjectiveFunction(const std::vector<int> &solution)
{
    assert(isClique(solution) && "Solution is not a clique.");
    return solution.size();
}

bool TabuSearch::isClique(const std::vector<int> &solution)
{
    adj_list_t adjacencyList = graph.get_adjacency_list();
    for (size_t i = 0; i < solution.size(); ++i)
    {
        for (size_t j = i + 1; j < solution.size(); ++j)
        {
            if (!graph.is_edge(solution[i], solution[j]))
            {
                return false;
            }
        }
    }
    return true;
}

std::vector<int> TabuSearch::getBestSolution() const
{
    return bestSolution;
}
