#include "ConstructiveAlgorithm.h"
#include <random>

using namespace std;
using namespace ConstructiveAlgorithm;

problem::ElementPtr ConstructiveAlgorithm::getBestElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);
    return listCandidates.front();
}

problem::ElementPtr ConstructiveAlgorithm::getElementAlphaSelection(problem::Instance& instance, problem::SolutionPtr solution, double alpha)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    auto element = listCandidates.front();
    if (rand() % 100 < (1 - alpha) * 100) {
        element = listCandidates.at(rand() % listCandidates.size());
    }

    return element;
}

void ConstructiveAlgorithm::selectBestCandidates(problem::Problem& problem, vector<problem::SolutionPtr>& solutions, vector<problem::SolutionPtr>& candidates)
{
    if (candidates.empty())
        return;

    int solutionSize = (int)solutions.size();
    solutions.clear();

    for (auto candidate : candidates)
    {
        if (solutions.size() < solutionSize)
        {
            solutions.push_back(candidate);
        }
        else
        {
            for (int i = 0; i < solutionSize; i++)
            {
                if (problem.objectiveValue(candidate) < problem.objectiveValue(solutions.at(i)))
                {
                    solutions.at(i) = candidate;
                    break;
                }
            }
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
    function<void(problem::Problem&, vector<problem::SolutionPtr>&, vector<problem::SolutionPtr>&)> selectionCandidateAlgorithm, 
    int beamWidth, 
    int expasionWidth)
{
    vector<problem::SolutionPtr> beam{};
    vector<problem::SolutionPtr> candidates{};
    problem::SolutionPtr bestSolution = nullptr;

    beam.reserve(beamWidth);
    candidates.reserve(beamWidth * expasionWidth);
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
                    auto newSolution = solution->clone();
                    newSolution->addElementToSolution(choosedElement);
                    candidates.push_back(newSolution);
                }
            }
        }
        selectionCandidateAlgorithm(problem, beam, candidates);
        candidates.clear();
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
