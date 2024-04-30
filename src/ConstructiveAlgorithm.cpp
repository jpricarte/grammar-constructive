#include "ConstructiveAlgorithm.h"
#include <random>

using namespace std;
using namespace ConstructiveAlgorithm;

problem::ElementPtr ConstructiveAlgorithm::getBestElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);
    return listCandidates.front();
}

problem::ElementPtr ConstructiveAlgorithm::getElementRandomSelection(problem::Instance& instance, problem::SolutionPtr solution, double alpha, int k, function<int(int)> random_distribuiton)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    auto element = listCandidates.front();
    if (rand() % 100 < (1 - alpha) * 100) {
        int max = (k <= 0 || k >= listCandidates.size()) ? listCandidates.size()-1 : k;
        int pos = random_distribuiton(max);
        element = listCandidates.at(pos);
    }

    return element;
}

void ConstructiveAlgorithm::selectBestCandidates(problem::Problem& problem, vector<problem::SolutionPtr>& solutions)
{
    for (auto solution : solutions)
    {
        double bestValue = INFINITY;
        problem::ElementPtr bestElement = nullptr;
        for (auto candidate : solution->getIterationOptions())
        {
            auto value = problem.objectiveValue(solution, candidate);
            if (value < bestValue) 
            {
                bestValue = value;
                bestElement = candidate;
            }
		}
        if (bestElement != nullptr)
        {
            solution->addElementToSolution(bestElement);
            solution->cleanIterationOptions();
        }
       
    }
}

problem::SolutionPtr ConstructiveAlgorithm::greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, 
    function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> selectionElementAlgorithm)
{
    auto solution = instance.initializeSolution();
    while (not problem.isComplete(instance, solution))
    {
        auto choosedElement = selectionElementAlgorithm(instance, solution);
        if (problem.isValid(instance, solution, choosedElement))
        {
            solution->addElementToSolution(choosedElement);
        }
        solution->addElementToVisited(choosedElement);
    }
    return solution;
}

problem::SolutionPtr ConstructiveAlgorithm::beamsearchAlgorithm(problem::Problem& problem, 
    problem::Instance& instance, 
    function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> selectionElementAlgorithm, 
    int beamWidth, 
    int expasionWidth)
{
    vector<problem::SolutionPtr> beam{};
    problem::SolutionPtr bestSolution = nullptr;

    beam.reserve(beamWidth);
    for (int i = 0; i < beamWidth; i++)
    {
        beam.push_back(instance.initializeSolution());
    }

    while (not beam.empty())
    {
        for (int i = beam.size() - 1; i >= 0; i--)
        {
            auto solution = beam.at(i);
            if (problem.isComplete(instance, solution))
            {
                if (bestSolution == nullptr or problem.objectiveValue(solution) < problem.objectiveValue(bestSolution))
                {
                    bestSolution = solution;
                }
                beam.erase(beam.begin() + i);
            }
            else
            {
                auto choosedElement = selectionElementAlgorithm(instance, solution);
                solution->addElementToVisited(choosedElement);
                if (problem.isValid(instance, solution, choosedElement))
                {
                    solution->addElementToIterationOptions(choosedElement);
                }
            }
        }
        selectBestCandidates(problem, beam);
    }
    return bestSolution;
}

problem::SolutionPtr ConstructiveAlgorithm::multistartAlgorithmMaxIterations(problem::Problem& problem, 
    problem::Instance& instance, 
    function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm,
    int numIterations)
{
    problem::SolutionPtr bestSolution = nullptr;
	for (int i = 0; i < numIterations; i++)
	{
		auto solution = algorithm(problem, instance);
		if (bestSolution == nullptr or problem.objectiveValue(solution) < problem.objectiveValue(bestSolution))
		{
			bestSolution = solution;
		}
	}
	return bestSolution;
}
