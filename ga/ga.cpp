#include <bitset>
#include <random>
#include <vector>
#include <iostream>
#include "ga.h"

std::bitset<NUM_MAX_VERTICES> GeneticAlgorithm::random_individual(int num_vertices)
{
    std::bitset<NUM_MAX_VERTICES> ind;
    for (int i = 0; i < num_vertices; ++i)
    {
        ind[i] = gen() % 2;
    }
    repair_clique(ind);
    return ind;
}

// Fitness function: return clique size if valid, otherwise 0
int GeneticAlgorithm::fitness(const std::bitset<NUM_MAX_VERTICES> &ind)
{

    std::vector<int> vertices;
    for (int i = 0; i < graph.get_number_of_vertices(); ++i)
    {
        if (ind[i])
            vertices.push_back(i);
    }
    assert(is_clique(vertices) and (vertices.size() == ind.count()) and "ERROR: Invalid clique");
    return ind.count();
}

// Check if the selected vertices form a clique in the graph
bool GeneticAlgorithm::is_clique(const std::vector<int> &vertices)
{
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        for (size_t j = i + 1; j < vertices.size(); ++j)
        {
            if (!graph.is_edge(vertices[i], vertices[j]))
            {
                return false;
            }
        }
    }
    return true;
}

// Single-point crossover to generate a child individual
std::pair<std::bitset<NUM_MAX_VERTICES>, std::bitset<NUM_MAX_VERTICES>> GeneticAlgorithm::crossover(const std::bitset<NUM_MAX_VERTICES> &parent1, const std::bitset<NUM_MAX_VERTICES> &parent2)
{
    int point = gen() % graph.get_number_of_vertices();
    std::bitset<NUM_MAX_VERTICES> child1, child2;
    for (int i = 0; i < point; ++i)
    {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }
    for (int i = point; i < graph.get_number_of_vertices(); ++i)
    {
        child1[i] = parent2[i];
        child2[i] = parent1[i];
    }
    return {child1, child2};
}

// Mutation: Flip a random bit in the individual
void GeneticAlgorithm::mutate(std::bitset<NUM_MAX_VERTICES> &individual)
{
    int index = gen() % graph.get_number_of_vertices();
    individual.flip(index);
}

void GeneticAlgorithm::generate_offspring(std::vector<std::bitset<NUM_MAX_VERTICES>> &prev_population)
{
    while ((int)prev_population.size() < 2 * population_size)
    {
        int idx_parent1 = gen() % population_size;
        int idx_parent2 = gen() % population_size;

        std::bitset<NUM_MAX_VERTICES> parent1 = population[idx_parent1];
        std::bitset<NUM_MAX_VERTICES> parent2 = population[idx_parent2];
        std::bitset<NUM_MAX_VERTICES> child1, child2;

        if (this->crossover_probability < dist(gen))
        {
            auto [c1, c2] = crossover(parent1, parent2);
            child1 = c1;
            child2 = c2;
        }
        else
        {
            child1 = parent1;
            child2 = parent2;
        }
        if (this->mutation_probability < dist(gen))
        {
            mutate(child1);
        }
        if (this->mutation_probability < dist(gen))
        {
            mutate(child2);
        }
        repair_clique(child1);
        repair_clique(child2);
        prev_population.push_back(child1);
        prev_population.push_back(child2);
    }
}

void GeneticAlgorithm::select_new_population(std::vector<std::bitset<NUM_MAX_VERTICES>> &parents_and_offspring)
{
    std::vector<int> fitnesses(2 * population_size);
#pragma omp parallel for
    for (int i = 0; i < 2 * population_size; i++)
    {
        fitnesses[i] = fitness(parents_and_offspring[i]);
    }
    std::vector<bool> selected(2 * population_size, false);
    for (int i = 0; i < population_size; i++)
    {
        std::set<int> participants;
        int winner_fitness = 0;
        int winner_idx = -1;
        for (size_t j = 0; j < 3; j++)
        {
            int idx = gen() % (2 * population_size);
            while (selected[idx] || participants.find(idx) != participants.end())
            {
                idx = gen() % (2 * population_size);
            }
            participants.insert(idx);
            if (fitnesses[idx] >= winner_fitness)
            {
                winner_fitness = fitnesses[idx];
                winner_idx = idx;
            }
        }
        selected[winner_idx] = true;
        population.push_back(parents_and_offspring[winner_idx]);
    }
}

// Generate the next generation of individuals
void GeneticAlgorithm::next_generation()
{
    std::vector<std::bitset<NUM_MAX_VERTICES>> parents_and_offspring = population;
    generate_offspring(parents_and_offspring);
    population.clear();
    select_new_population(parents_and_offspring);
}

void GeneticAlgorithm::repair_clique(std::bitset<NUM_MAX_VERTICES> &individual)
{
    std::vector<int> vertices;
    for (int i = 0; i < graph.get_number_of_vertices(); ++i)
    {
        if (individual[i])
            vertices.push_back(i);
    }
    std::map<int, std::vector<int>> neighbours;
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        for (size_t j = i + 1; j < vertices.size(); ++j)
        {
            if (graph.is_edge(vertices[i], vertices[j]))
            {
                neighbours[vertices[i]].push_back(vertices[j]);
                neighbours[vertices[j]].push_back(vertices[i]);
            }
        }
        if (neighbours.find(vertices[i]) == neighbours.end())
            neighbours[vertices[i]] = std::vector<int>();
    }
    while (!is_clique(vertices))
    {
        assert(vertices.size() == neighbours.size() and "ERROR: Invalid clique");
        int min_degree = graph.get_number_of_vertices() + 10;
        int vertex_to_remove = -1;
        for (const auto &[vertex, neighbour_list] : neighbours)
        {
            if ((int)neighbour_list.size() < min_degree)
            {
                min_degree = neighbour_list.size();
                vertex_to_remove = vertex;
            }
        }
        if (vertex_to_remove == -1)
        {
            std::cout << "ERROR: No vertex to remove" << std::endl;
            std::cout << vertices.size() << std::endl;
            std::cout << neighbours.size() << std::endl;
            std::cout << min_degree << std::endl;
            std::cout << "Vertices:" << std::endl;
            for (const auto &[vertex, neighbour_list] : neighbours)
            {
                std::cout << vertex << " " << neighbour_list.size() << std::endl;
            }
        }
        assert(vertex_to_remove != -1 and "ERROR: No vertex to remove");

        individual[vertex_to_remove] = 0;
        vertices.erase(std::remove(vertices.begin(), vertices.end(), vertex_to_remove), vertices.end());
        neighbours.erase(vertex_to_remove);
        for (auto &[vertex, neighbour_list] : neighbours)
        {
            neighbour_list.erase(std::remove_if(neighbour_list.begin(), neighbour_list.end(),
                                                [vertex_to_remove](int v)
                                                { return v == vertex_to_remove; }),
                                 neighbour_list.end());
        }
    }
}

// Constructor: Initializes the genetic algorithm with the given graph, population size, mutation rate, and number of generations
GeneticAlgorithm::GeneticAlgorithm(const Graph &g, int pop_size, double crossover_probability, double mutation_probability, int gens)
    : graph(g), population_size(pop_size), crossover_probability(crossover_probability), mutation_probability(mutation_probability), generations(gens), gen(rd()), dist(0.0, 1.0) {}

// Run the genetic algorithm and return the best clique found as a vector of vertex indices
std::vector<int> GeneticAlgorithm::run()
{
    int num_vertices = graph.get_number_of_vertices();
    population.clear();
    population.resize(population_size);
    #pragma omp parallel for
    for (int i = 0; i < population_size; ++i)
    {
        population[i] = random_individual(num_vertices);
    }

    // Evolution loop
    for (int generation = 0; generation < generations; ++generation)
    {
        if (generation % 10 == 0)
            std::cout << "Generation " << generation << std::endl;
        next_generation();
    }
    std::bitset<NUM_MAX_VERTICES> best_individual;
    int best_fitness = 0;

    for (int i = 0; i < population_size; i++)
    {
        int f = fitness(population[i]);
        if (f >= best_fitness)
        {
            best_fitness = f;
            best_individual = population[i];
        }
    }

    std::vector<int> clique;
    for (int i = 0; i < graph.get_number_of_vertices(); ++i)
    {
        if (best_individual[i])
            clique.push_back(i);
    }

    return clique;
}
