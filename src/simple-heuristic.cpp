// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"
#include "Problem.h"

#include <functional>
#include <random>

using namespace std;

int autoKLSFP(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " <seed> <instance> <configuration>" << endl;
		return 1;
	}

    return autoKLSFP(argc, argv);
    //return autoKnapsack(argc, argv);
    //return manualKnapsack(argc, argv);
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
    cout << solution->getObjectiveValue() << endl;

    return 0;
}

int autoKnapsack(int argc, char* argv[])
{
    srand(0xc0ffe3);
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[2]);
    auto configuration = AlgorithmConfiguration();
    configuration.readConfiguration(argv[3]);

    auto solution = dynamic_pointer_cast<KnapsackSolution>(configuration.run(problem, instance));
    cout << solution->getObjectiveValue() << " " << solution->getCurrentWeight() << endl;
    for (auto element : solution->getSolution())
    {
        auto knapsackElement = dynamic_pointer_cast<KnapsackElement>(element);
        cout << knapsackElement->value << " " << knapsackElement->weight << endl;
    }
    
    return 0;
}
