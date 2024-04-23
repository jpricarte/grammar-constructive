#include "ConstructiveAlgorithm.h"

using namespace std;
using namespace ConstructiveAlgorithm;

shared_ptr<problem::Element> ConstructiveAlgorithm::getBestElement(problem::Instance& instance, shared_ptr<problem::Solution> solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);
    return listCandidates.front();
}

shared_ptr<problem::Element> ConstructiveAlgorithm::getElementAlphaSelection(problem::Instance& instance, shared_ptr<problem::Solution> solution, double alpha)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    auto element = listCandidates.front();
    if (rand() % 100 < (1 - alpha) * 100) {
        element = listCandidates.at(rand() % listCandidates.size());
    }

    return element;
}

void ConstructiveAlgorithm::selectBestCandidates(problem::Problem& problem, vector<std::shared_ptr<problem::Solution>>& solutions, vector<std::shared_ptr<problem::Solution>>& candidates)
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

shared_ptr<problem::Solution> ConstructiveAlgorithm::greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, 
    function<shared_ptr<problem::Element>(problem::Instance&, shared_ptr<problem::Solution>)> selectionElementAlgorithm)
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

shared_ptr<problem::Solution> ConstructiveAlgorithm::beamsearchAlgorithm(problem::Problem& problem, 
    problem::Instance& instance, 
    function<shared_ptr<problem::Element>(problem::Instance&, shared_ptr<problem::Solution>)> selectionElementAlgorithm, 
    function<void(problem::Problem&, vector<shared_ptr<problem::Solution>>&, vector<shared_ptr<problem::Solution>>&)> selectionCandidateAlgorithm, 
    int beamWidth, 
    int expasionWidth)
{
    vector<std::shared_ptr<problem::Solution>> beam{};
    vector<std::shared_ptr<problem::Solution>> candidates{};
    shared_ptr<problem::Solution> bestSolution = nullptr;

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

shared_ptr<problem::Solution> ConstructiveAlgorithm::multistartAlgorithmMaxIterations(problem::Problem& problem, 
    problem::Instance& instance, 
    function<shared_ptr<problem::Solution>(problem::Problem&, problem::Instance&)> algorithm,
    int numIterations)
{
    shared_ptr<problem::Solution> bestSolution = nullptr;
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
