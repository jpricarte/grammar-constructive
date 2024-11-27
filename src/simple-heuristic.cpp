// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"
#include "../submodule/fssp/instance.hpp"
#include "Problem.h"
#include <fstream>
#include <filesystem>

#define DEBUG 0

// Defined global in Problem.h
uint64_t gOperationCounter = 0;

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        cout << "Usage: " << argv[0] << " <seed> <instance> <configuration> <problem>" << endl;
        return 1;
    }

    if (std::string(argv[4]) == "fssp")
        return autoFSSP(argc, argv);
    if (std::string(argv[4]) == "klsfp")
        return autoKLSFP(argc, argv);
    if (std::string(argv[4]) == "knapsack")
        return autoKnapsack(argc, argv);
}

int autoFSSP(int argc, char* argv[])
{
    srand(atoi(argv[1]));
    string instanceName = argv[2];
    std::ifstream isInstance(instanceName);
    InstanceFSSP instanceFSSP{isInstance};

    auto problem = FSSProblem();
    auto instance = FSSInstance(instanceFSSP);
    auto configuration = AlgorithmConfiguration();
    configuration.readConfiguration(argv[3]);
    auto solution = static_pointer_cast<FSSSolution>(configuration.run(problem, instance));

    cout << solution->getObjectiveValue() << endl;

    if (DEBUG)
    {
        for (int i=0; i<solution->getSolution().size(); i++)
            cout << static_pointer_cast<FSSElement>(solution->getSolution()[i])->jobNumber << " ";
        cout << endl;
    }

    return 0;
}

int autoKLSFP(int argc, char* argv[])
{
    srand(atoi(argv[1]));
    auto problem = KLSFProblem();
    auto graph = Graph(argv[2]);
    auto instance = KLSFInstance(graph);
    auto configuration = AlgorithmConfiguration();
    configuration.readConfiguration(argv[3]);

    auto solution = dynamic_pointer_cast<KLSFSolution>(configuration.run(problem, instance));
    //cout << gOperationCounter << endl;
    // config,instance,obj_val,operations
    cout << solution->getObjectiveValue() /*<< "," << fs::path(argv[3]).filename() << "," << fs::path(argv[2]).filename() << "," << gOperationCounter*/ << endl;

    return 0;
}

int autoKnapsack(int argc, char* argv[])
{
    srand(atoi(argv[1]));
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[2]);
    auto configuration = AlgorithmConfiguration();
    configuration.readConfiguration(argv[3]);

    auto solution = dynamic_pointer_cast<KnapsackSolution>(configuration.run(problem, instance));
    cout << solution->getObjectiveValue() << endl;
    
    return 0;
}
