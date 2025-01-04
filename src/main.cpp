#include "../ga/ga.h"
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
  GeneticAlgorithm ga(g, 500, 0.9, 0.1, 1000);
  std::cout << "Running genetic algorithm..." << std::endl;
  std::vector<int> clique = ga.run();
  std::cout << "Clique size: " << clique.size() << std::endl;
  exit(EXIT_SUCCESS);
}
