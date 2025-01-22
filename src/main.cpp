#include "../ga/ga.h"
#include "../ts/tabusearch.h"
#include "../sa/SimulatedAnnealing.h"
#include <cstdlib>
#include <string>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <dirent.h>

std::string extract_file_name(const std::string &file_path)
{
	// Encontra a posição do último separador de diretório
	size_t last_slash_pos = file_path.find_last_of("/\\");
	// Extrai o nome do arquivo com extensão
	std::string file_name = file_path.substr(last_slash_pos + 1);
	// Encontra a posição do último ponto
	size_t last_dot_pos = file_name.find_last_of(".");
	// Remove a extensão
	if (last_dot_pos != std::string::npos)
	{
		file_name = file_name.substr(0, last_dot_pos);
	}
	return file_name;
}

void control_test(const std::string &graph_file, int num_executions, const std::string &output_file)
{
	std::ofstream out(output_file, std::ios::app);
	if (!out.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de output: " << output_file << std::endl;
		return;
	}
	std::string graph_file_pretty = extract_file_name(graph_file);
	Graph g;
	g.read_edge_list(graph_file);

	int best_clique = 0;
	double total_time = 0.0;
	for (int i = 0; i < num_executions; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();

		SimulatedAnnealing sa(100.0, 0.001, 0.9995, g, 1);
		std::cout << "Running simulated annealing algorithm..." << std::endl;
		std::vector<int> clique = sa.run();
		int clique_size_sa = clique.size();
		std::cout << "Clique size sa: " << clique_size_sa << std::endl;

		int cur_clique = clique_size_sa;
		if (cur_clique > best_clique)
		{
			best_clique = cur_clique;
		}
		auto end = std::chrono::high_resolution_clock::now();

		total_time += std::chrono::duration<double>(end - start).count();
		double exec_time = std::chrono::duration<double>(end - start).count();

		out << (i + 1) << "," << graph_file_pretty << "," << cur_clique << "," << exec_time << std::endl;
	}
	std::cout << "Melhor clique em 20 execucoes = " << best_clique << std::endl;
	std::cout << "Tempo de execucao total: " << total_time << " seconds" << std::endl;
}

void test(const std::string &graph_file, int num_executions, const std::string &output_file)
{
	std::ofstream out(output_file, std::ios::app);
	if (!out.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de output: " << output_file << std::endl;
		return;
	}
	std::string graph_file_pretty = extract_file_name(graph_file);
	Graph g;
	g.read_edge_list(graph_file);

	int best_clique = 0;
	double total_time = 0.0;
	for (int i = 0; i < num_executions; ++i)
	{
		auto start = std::chrono::high_resolution_clock::now();

		GeneticAlgorithm ga(g, 100, 0.9, 0.1, 100);
		std::cout << "Running genetic algorithm..." << std::endl;
		auto start_ga = std::chrono::high_resolution_clock::now();
		std::vector<int> clique = ga.run();
		auto end_ga = std::chrono::high_resolution_clock::now();
		int clique_size_ga = clique.size();
		std::cout << "Clique size ga: " << clique_size_ga << std::endl;
		SimulatedAnnealing sa(100.0, 0.001, 0.9995, g, clique_size_ga, clique);
		std::cout << "Running simulated annealing algorithm..." << std::endl;
		auto start_sa = std::chrono::high_resolution_clock::now();
		clique = sa.run();
		auto end_sa = std::chrono::high_resolution_clock::now();
		int clique_size_sa = clique.size();
		std::cout << "Clique size sa: " << clique_size_sa << std::endl;

		std::cout << "Running tabu search..." << std::endl;
		TabuSearch ts(g, clique, 10, 100);
		auto start_ts = std::chrono::high_resolution_clock::now();
		ts.run();
		auto end_ts = std::chrono::high_resolution_clock::now();

		int cur_clique = ts.getBestClique();
		std::cout << "Clique size ts: " << cur_clique << std::endl;
		std::cout << "Done" << std::endl;
		if (cur_clique > best_clique)
		{
			best_clique = cur_clique;
		}
		auto end = std::chrono::high_resolution_clock::now();

		total_time += std::chrono::duration<double>(end - start).count();
		double exec_time = std::chrono::duration<double>(end - start).count();
		double exec_time_ga = std::chrono::duration<double>(end_ga - start_ga).count();
		double exec_time_sa = std::chrono::duration<double>(end_sa - start_sa).count();
		double exec_time_ts = std::chrono::duration<double>(end_ts - start_ts).count();

		out << (i + 1) << "," << graph_file_pretty << "," << clique_size_ga << ","
			<< clique_size_sa << "," << cur_clique << "," << exec_time << "," 
			<< exec_time_ga<<"," << exec_time_sa<<"," << exec_time_ts<<std::endl;
	}
	std::cout << "Melhor clique em 20 execucoes = " << best_clique << std::endl;
	std::cout << "Tempo de execucao total: " << total_time << " seconds" << std::endl;
}


void grid_search_ga(const std::string &graph_file, int num_executions, const std::string &output_file)
{
	std::ofstream out(output_file, std::ios::app);
	if (!out.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de output: " << output_file << std::endl;
		return;
	}
	std::string graph_file_pretty = extract_file_name(graph_file);
	Graph g;
	g.read_edge_list(graph_file);

	std::vector<double> list_pcs = {0.9, 0.8, 0.7};
	std::vector<double> list_pms = {0.3, 0.2, 0.1};
	std::vector<int> list_pop = {100, 200, 300};
	std::vector<int> list_gens = {100, 200, 300};
	for (auto const pc : list_pcs)
	{
		for (auto const pm : list_pms)
		{
			for (auto const pop : list_pop)
			{
				for (auto const gens : list_gens)
				{
					for (int i = 0; i < num_executions; ++i)
					{
						auto start = std::chrono::high_resolution_clock::now();
						GeneticAlgorithm ga(g, pop, pc, pm, gens);
						std::cout << "Running genetic algorithm..." << std::endl;
						std::vector<int> clique = ga.run();
						int clique_size_ga = clique.size();
						std::cout << "Clique size ga: " << clique_size_ga << std::endl;
						double exec_time_ga = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
						out << (i + 1) << "," << graph_file_pretty << "," << clique_size_ga << "," << exec_time_ga << ","
							<< pc << "," << pm << "," << pop << "," << gens << std::endl;
					}
				}
			}
		}
	}
}

void test_all_files_in_directory(const std::string &directory_path, int num_executions, const std::string &output_file)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(directory_path.c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string file_name = ent->d_name;
			if (file_name != "." && file_name != "..")
			{
				std::string file_path = directory_path + "/" + file_name;
				std::cout << "Testing file: " << file_path << std::endl;
				test(file_path, num_executions, output_file);
			}
		}
		closedir(dir);
	}
	else
	{
		std::cerr << "Erro ao abrir o diretório: " << directory_path << std::endl;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <directory path>" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string directory_path = argv[1];
	int num_executions = 20;
	std::string output_file = "heruristics_test.csv";

	test_all_files_in_directory(directory_path, num_executions, output_file);

	return 0;
}