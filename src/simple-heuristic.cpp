// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);

    function<std::shared_ptr<problem::Element>(problem::Instance&, std::shared_ptr<problem::Solution>)> f = [] (problem::Instance& i, shared_ptr<problem::Solution> s) -> std::shared_ptr<problem::Element> {
        return ConstructiveAlgorithm::getElementAlphaSelection(i, s, 0.8); 
    };

    double value = ConstructiveAlgorithm::greedyAlgorithm(problem, instance, f);

    cout << "Objective value: " << value << endl;

    return 0;
}
