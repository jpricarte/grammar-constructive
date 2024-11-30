// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"
#include "../submodule/fssp/instance.hpp"
#include "Problem.h"
#include "parser.h"
#include <cfloat>
#include <fstream>
#include <filesystem>
#include <climits>

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

void setBugetFSSP(AlgorithmConfiguration& configuration, FSSInstance& instance)
{
    int m = instance.getNumMachines();
    int n = instance.getNumJobs();
    if (m == 5 && n <= 20)
    {
        // configuration.stopCriteria->maxIterations = 0;
        // configuration.stopCriteria->maxNoImprov = 0;
        configuration.stopCriteria->maxBudget = 700;
    }
    else if (n <= 200)
    {
        // configuration.stopCriteria->maxIterations = 0;
        // configuration.stopCriteria->maxNoImprov = 0;
        configuration.stopCriteria->maxBudget = (int) (m * 5418.36)+(n*189.1)-31093.04;
    }
    else
    {
        // configuration.stopCriteria->maxIterations = 0;
        // configuration.stopCriteria->maxNoImprov = 0;
        configuration.stopCriteria->maxBudget = (int) (m * 16537.1)+(n*262.73)-102886.16;

    }
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
    setBugetFSSP(configuration, instance);
    auto solution = static_pointer_cast<FSSSolution>(configuration.run(problem, instance));

    double value = DBL_MAX;
    if (solution)
    {
        value = solution->getObjectiveValue();
    }

    // config,instance,obj_val,operations
    cout << fs::path(argv[3]).filename() << "," << fs::path(argv[2]).filename() << "," << value << "," << gOperationCounter << endl;

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
    cout << fs::path(argv[3]).filename() << "," << fs::path(argv[2]).filename() << "," << solution->getObjectiveValue() << "," << gOperationCounter << endl;

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
