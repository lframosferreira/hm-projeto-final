#include "SimulatedAnnealing.h"



SimulatedAnnealing::SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize, vector<int> perm)
    : initialTemperature(initialTemp), endTemperature(endTemp), coolingCoefficient(coolingCoeff), currentTemperature(initialTemp),
      graph(graph), m(cliqueSize), n(graph.get_number_of_vertices()), permutation(perm), last_clique(perm), rng(random_device{}()) {
    
    initialize(perm);
    
}

SimulatedAnnealing::SimulatedAnnealing(double initialTemp, double endTemp, double coolingCoeff, Graph graph, int cliqueSize)
    : initialTemperature(initialTemp), endTemperature(endTemp), coolingCoefficient(coolingCoeff), currentTemperature(initialTemp),
      graph(graph), m(cliqueSize), n(graph.get_number_of_vertices()), rng(random_device{}()) {
    initialize();

}

void SimulatedAnnealing::initialize() {
    last_clique = {};
    permutation.resize(n);
    iota(permutation.begin(), permutation.end(), 0);
    random_device rd;
    mt19937 gen(rd());
    shuffle(permutation.begin(), permutation.end(), gen);
    setupDegrees();

}

void SimulatedAnnealing::initialize(vector<int> perm) {
    set<int> permSet(perm.begin(), perm.end());

    for (int i = 0; i < n; ++i) {
        if (permSet.find(i) == permSet.end()) {
            permutation.push_back(i);
        }
    }
    setupDegrees();
}

void SimulatedAnnealing::setupDegrees() {
    vertexDegrees.resize(n); // 0-based indexing
    auto adjacency_list = graph.get_adjacency_list();
    for (int i = 0; i < n; ++i) {
        vertexDegrees[i] = adjacency_list[i].size();
    }
}

void SimulatedAnnealing::adjustPermutation() {
    sort(permutation.begin(), permutation.begin() + m, [this](int a, int b) {
        return a < b;
    });
}

pair<int, int> SimulatedAnnealing::selectVertices() {
    uniform_int_distribution<int> cliqueDist(0, m - 1);
    uniform_int_distribution<int> nonCliqueDist(m, n - 1);

    int attempts = 0;
    int u = cliqueDist(rng);
    while (attempts < 8 * n) {
        int w = nonCliqueDist(rng);

        double f0u = computePartialObjective(u);
        double f0w = computePartialObjective(w);

        if (f0u <= f0w || ++attempts >= 8 * n) {
            return {u, w};
        }
    }

    return {0, m}; // Default fallback
}

double SimulatedAnnealing::computeObjectiveFunction(const vector<int>& perm) {
    double score = 0;
    for (int i = 0; i < m-1; i++) {
        for (int j = i + 1; j < m; j++) {
            int u = perm[i], v = perm[j];
            if (!graph.is_edge(u, v)){
                score += 1;
            }
        }
    }

    return score;
}

double SimulatedAnnealing::computePartialObjective(int vertexIndex) {
    double score = 0;
    for (int i = 0; i < m; ++i) {
        if (i != vertexIndex) {
            int u = permutation[vertexIndex];
            int v = permutation[i];
            if (graph.is_edge(u, v)){
                score += 1;
            }
            
        }
    }
    return score;
}

void SimulatedAnnealing::performStateTransition(int u, int w) {
    swap(permutation[u], permutation[w]);
}

bool SimulatedAnnealing::acceptNewState(double deltaF) {
    if (deltaF <= 0) {
        return true;
    }
    double probability = exp(-deltaF / currentTemperature);
    uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng) < probability;
}

std::vector<int> SimulatedAnnealing::run(){
    currentF = 0;
    vector<int> perm;

    while (currentF == 0){
        perm = maximum_clique();
        m+=1;
    }
    return perm;
}

std::vector<int> SimulatedAnnealing::maximum_clique() {
    
    currentF = computeObjectiveFunction(permutation);


    while (currentTemperature > endTemperature) {
        if (currentF == 0) {
            last_clique = vector<int>(permutation.begin(), permutation.begin() + m);
            break; // Solution found
        }
        auto [u, w] = selectVertices();
        performStateTransition(u, w);

        double newF = computeObjectiveFunction(permutation);
        double deltaF = newF - currentF;

        if (acceptNewState(deltaF)) {
            currentF = newF;
        } else {
            performStateTransition(u, w); // Revert the change
        }


        currentTemperature *= coolingCoefficient;

    }

    cout << "Final objective function value: " << currentF <<" clique size: "<< m<< endl;
	if(currentF == 0)
		return vector<int>(permutation.begin(), permutation.begin() + m);
	else
		return last_clique;
}

