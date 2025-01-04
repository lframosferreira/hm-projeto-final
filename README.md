# Heurísticas e Metaheurísticas
## Como compilar/executar o programa
``g++ -g -fopenmp -c src/main.cpp -o src/main.o``

``g++ -g -fopenmp -c ga/ga.cpp -o ga/ga.o``

``g++ -g -fopenmp -c graph/graph.cpp -o graph/graph.o``

``g++ -g -fopenmp -o main src/main.o ga/ga.o graph/graph.o``

``./main ./DIMACS/{nome_arquivo_clique}``
