// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);

    function<std::shared_ptr<problem::Element>(problem::Instance&, std::shared_ptr<problem::Solution>)> elementselection = [] (problem::Instance& i, shared_ptr<problem::Solution> s) -> std::shared_ptr<problem::Element> {
        return ConstructiveAlgorithm::getBestElement(i, s); 
    };

    function<void(problem::Problem&, std::vector<std::shared_ptr<problem::Solution>>&, std::vector<std::shared_ptr<problem::Solution>>&)> solutionSelection = [](problem::Problem& p, std::vector<std::shared_ptr<problem::Solution>>& s, std::vector<std::shared_ptr<problem::Solution>>& c) {
        return ConstructiveAlgorithm::selectBestCandidates(p, s, c);
    };


    shared_ptr<KnapsackSolution> solution = dynamic_pointer_cast<KnapsackSolution>(ConstructiveAlgorithm::beamsearchAlgorithm(problem, instance, elementselection, solutionSelection, 2, 2));

    cout << "Objective value: " << solution->getObjectiveValue() << endl;

    return 0;
}
