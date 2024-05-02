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
    default_random_engine generator;

    function<int(int)> random_distribuiton = [&](int max) -> int {
        uniform_int_distribution<int> distribution(0, max);
		return distribution(generator);
	};

    function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> elementselection = [&] (problem::Instance& i, problem::SolutionPtr s) -> problem::ElementPtr {
        return ConstructiveAlgorithm::getElementRandomSelection(i, s, 0.6, 5, random_distribuiton);
    };

    function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm = [&] (problem::Problem& p, problem::Instance& i) {
        return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, elementselection, 2, 5);
        //return ConstructiveAlgorithm::greedyAlgorithm(p, i, elementselection);
	};


    auto solution = dynamic_pointer_cast<KnapsackSolution>(ConstructiveAlgorithm::multistartAlgorithmMaxIterations(problem, instance, algorithm, 100));
    //problem::SolutionPtr solution = dynamic_pointer_cast<KnapsackSolution>(algorithm(problem, instance));

    cout << solution->getObjectiveValue() << " " << solution->getCurrentWeight() << endl;
    for (auto element : solution->getSolution())
    {
		auto knapsackElement = dynamic_pointer_cast<KnapsackElement>(element);
		cout << knapsackElement->value << " " << knapsackElement->weight << endl;
	}

    return 0;
}
