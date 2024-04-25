// simple-heuristic.cpp: define o ponto de entrada para o aplicativo.
//

#include "simple-heuristic.h"

#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
    srand(0xc0ff33);
    auto problem = KnapsackProblem();
    auto instance = KnapsackInstance(argv[1]);

    function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> elementselection = [] (problem::Instance& i, problem::SolutionPtr s) -> problem::ElementPtr {
        return ConstructiveAlgorithm::getElementAlphaSelection(i, s, 1); 
    };

    function<void(problem::Problem&, std::vector<problem::SolutionPtr>&, std::vector<problem::SolutionPtr>&)> solutionSelection = [](problem::Problem& p, std::vector<problem::SolutionPtr>& s, std::vector<problem::SolutionPtr>& c) {
        return ConstructiveAlgorithm::selectBestCandidates(p, s, c);
    };

    function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm = [&] (problem::Problem& p, problem::Instance& i) {
        return ConstructiveAlgorithm::beamsearchAlgorithm(p, i, elementselection, solutionSelection, 2, 5);
	};


    shared_ptr<KnapsackSolution> solution = dynamic_pointer_cast<KnapsackSolution>(ConstructiveAlgorithm::multistartAlgorithmMaxIterations(problem, instance, algorithm, 100));

    cout << solution->getObjectiveValue() << endl;

    return 0;
}
