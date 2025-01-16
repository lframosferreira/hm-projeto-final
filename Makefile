CXX = g++
CXXFLAGS = -g -fopenmp -Werror -O3 -Wall

SRC_DIR = src
GA_DIR = ga
GRAPH_DIR = graph
TS_DIR = ts
SA_DIR = sa

SRC_FILES = $(SRC_DIR)/main.cpp $(GA_DIR)/ga.cpp $(GRAPH_DIR)/graph.cpp $(TS_DIR)/tabusearch.cpp $(SA_DIR)/SimulatedAnnealing.cpp
OBJ_FILES = $(SRC_DIR)/main.o $(GA_DIR)/ga.o $(GRAPH_DIR)/graph.o $(TS_DIR)/tabusearch.o $(SA_DIR)/SimulatedAnnealing.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GA_DIR)/%.o: $(GA_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GRAPH_DIR)/%.o: $(GRAPH_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TS_DIR)/%.o: $(TS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SA_DIR)/%.o: $(SA_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_FILES) $(TARGET)

run: $(TARGET)
	./$(TARGET) $(clique_file)
