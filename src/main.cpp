#include "../ga/ga.h"
#include "../ts/tabusearch.h"
#include "../sa/SimulatedAnnealing.h"
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <graph file>" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string filename = argv[1];

  Graph g;
  g.read_edge_list(filename);
  std::cout << filename << std::endl;
  
  const int num_executions = 20;
  int best_clique = 0;
  double total_time = 0.0;
  for (int i = 0; i < num_executions; ++i)
  {
	  auto start = std::chrono::high_resolution_clock::now();
	  GeneticAlgorithm ga(g, 100, 0.9, 0.1, 100);
	  std::cout << "Running genetic algorithm..." << std::endl;
	  std::vector<int> clique = ga.run();
	  std::cout << "Clique size (ga): " << clique.size() << std::endl;
	  SimulatedAnnealing sa(100.0, 0.001, 0.9995, g, 34,clique);
	  std::cout << "Running simulated annealing algorithm..." << std::endl;
	  clique = sa.run();
	  TabuSearch ts(g, clique, 10, 100);
	  std::cout << "Running tabu search..." << std::endl;
	  ts.run();
	  std::cout << "Done" << std::endl;
	  int cur_clique = ts.getBestClique();
	  if (cur_clique > best_clique)
	  {
		best_clique = cur_clique;
	  }
	  auto end = std::chrono::high_resolution_clock::now();
	  total_time += std::chrono::duration<double>(end - start).count();
  }
  std::cout << "Melhor clique em 20 execucoes = " << best_clique << std::endl;
  std::cout << "Tempo de execucao medio: " << total_time / num_executions << " seconds" << std::endl;
  exit(EXIT_SUCCESS);
}
