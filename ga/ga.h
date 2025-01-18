#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <bitset>
#include <vector>
#include <random>
#include "../graph/graph.h"
#include <cassert>
#include <set>
#include <map>
#include <chrono>
#include <omp.h>

class GeneticAlgorithm
{
private:
    const Graph &graph;
    int population_size;
    double crossover_probability;
    double mutation_probability;
    int generations;

    std::vector<std::bitset<NUM_MAX_VERTICES>> population; // Population of bitsets
    std::random_device rd;                                 // Random device for random number generation
    std::mt19937 gen;                                      // Mersenne Twister random number generator
    std::uniform_real_distribution<double> dist;
    // Generate a random individual represented by a bitset
    std::bitset<NUM_MAX_VERTICES> random_individual(int num_vertices);

    // Fitness function: return clique size if valid, otherwise 0
    int fitness(const std::bitset<NUM_MAX_VERTICES> &ind);

    // Check if the selected vertices form a clique in the graph
    bool is_clique(const std::vector<int> &vertices);


    // Single-point crossover to generate a child individual
    // Single-point crossover to generate a child individual
    std::pair<std::bitset<NUM_MAX_VERTICES>, std::bitset<NUM_MAX_VERTICES>> crossover(const std::bitset<NUM_MAX_VERTICES> &parent1, const std::bitset<NUM_MAX_VERTICES> &parent2);

    // Mutation: Flip random bits in the individual based on mutation rate
    void mutate(std::bitset<NUM_MAX_VERTICES> &individual);

    void generate_offspring(std::vector<std::bitset<NUM_MAX_VERTICES>> &prev_population);

    void select_new_population(std::vector<std::bitset<NUM_MAX_VERTICES>> &parents_and_offspring);
    // Generate the next generation of individuals
    void next_generation();

    void repair_clique(std::bitset<NUM_MAX_VERTICES> &individual);

public:
    // Constructor: Initializes the genetic algorithm with the given graph, population size, mutation rate, and number of generations
    GeneticAlgorithm(const Graph &g, int pop_size, double crossover_probability, double mutation_probability, int gens);

    // Run the genetic algorithm and return the best clique found as a vector of vertex indices
    std::vector<int> run();
};

#endif // GENETIC_ALGORITHM_H
