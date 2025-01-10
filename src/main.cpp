#include "../ga/ga.h"
#include "../ts/tabusearch.h"
#include <cstdlib>
#include <string>
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
  GeneticAlgorithm ga(g, 100, 0.9, 0.1, 100);
  std::cout << "Running genetic algorithm..." << std::endl;
  std::vector<int> clique = ga.run();
  std::cout << "Clique size (ga): " << clique.size() << std::endl;
  TabuSearch ts(g, clique, 10, 100);
  std::cout << "Running tabu search..." << std::endl;
  ts.run();
  std::cout << "Done" << std::endl;
  exit(EXIT_SUCCESS);
}
