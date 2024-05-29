// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>
#include <random>

using namespace std;


int main(int argc, char* argv[])
{
    srand(0xc0ffe3);
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);
    auto configuration = AlgorithmConfiguration();
    configuration.readConfiguration(argv[2]);

    auto solution = dynamic_pointer_cast<KnapsackSolution>(configuration.run(problem, instance));
    cout << solution->getObjectiveValue() << " " << solution->getCurrentWeight() << endl;
    for (auto element : solution->getSolution())
    {
        auto knapsackElement = dynamic_pointer_cast<KnapsackElement>(element);
        cout << knapsackElement->value << " " << knapsackElement->weight << endl;
    }
    
    return 0;
}


int old_main(int argc, char* argv[])
{
    srand(0xc0ffe3);
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);
    default_random_engine generator;

    auto selector = make_shared<selection::RandomSelector>(0, 0);

    function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm = [&] (problem::Problem& p, problem::Instance& i) {
        //return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, selector, 4, 4);
        return ConstructiveAlgorithm::greedyAlgorithm(p, i, selector);
	};


    //auto solution = dynamic_pointer_cast<KnapsackSolution>(ConstructiveAlgorithm::multistartAlgorithmMaxIterations(problem, instance, algorithm, 100));
    auto solution = dynamic_pointer_cast<KnapsackSolution>(algorithm(problem, instance));

    cout << solution->getObjectiveValue() << " " << solution->getCurrentWeight() << endl;
    for (auto element : solution->getSolution())
    {
		auto knapsackElement = dynamic_pointer_cast<KnapsackElement>(element);
		cout << knapsackElement->value << " " << knapsackElement->weight << endl;
	}

    return 0;
}
