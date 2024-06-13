#include "ConstructiveAlgorithm.h"
#include <random>

using namespace std;
using namespace ConstructiveAlgorithm;


bool ConstructiveAlgorithm::StopCriteria::shouldStop(int numIterations, int numNoImprov)
{
    return (numIterations >= this->maxIterations or numNoImprov >= this->maxNoImprovementIterations);
}

void ConstructiveAlgorithm::selectBestCandidates(problem::Problem& problem, vector<problem::SolutionPtr>& solutions)
{
    using Option = tuple<problem::SolutionPtr, problem::ElementPtr, double>;

    auto solutionSize = solutions.size();
    vector<Option> bestOptions{};

    for (auto solution : solutions)
    {
        for (auto candidate : solution->getIterationOptions())
        {
            bestOptions.push_back(make_tuple(solution, candidate, problem.objectiveValue(solution)));
		}       
    }

    if (not bestOptions.empty()) {
        sort(bestOptions.begin(), bestOptions.end(), [](Option a, Option b) {
            return get<2>(a) < get<2>(b);
        });

        solutions.clear();
        for (int i = 0; i < solutionSize; i++)
        {
            if (bestOptions.size() <= i) break;

            auto option = bestOptions.at(i);
            auto solution = get<0>(option);
            auto candidate = get<1>(option);
            auto newSolution = solution->clone();
            newSolution->addElementToSolution(candidate);
            newSolution->addElementToVisited(candidate);
            solutions.push_back(newSolution);
        }
    }
}

problem::SolutionPtr ConstructiveAlgorithm::greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector)
{
    auto solution = instance.initializeSolution();
    while (not problem.isComplete(instance, solution))
    {
        auto choosedElement = selector->selectElement(instance, solution);
        if (problem.isValid(instance, solution, choosedElement))
        {
            solution->addElementToSolution(choosedElement);
        }
        solution->addElementToVisited(choosedElement);
    }
    return solution;
}

problem::SolutionPtr ConstructiveAlgorithm::beamsearchAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector, int beamWidth, int expasionWidth)
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
        for (auto i = static_cast<int>(beam.size()) - 1; i >= 0; i--)
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
                for (int j = 0; j < expasionWidth; j++)
                {
                    auto choosedElement = selector->selectElement(instance, solution);

                    if (choosedElement == nullptr) break;

                    if (problem.isValid(instance, solution, choosedElement))
                    {
                        if (not solution->getIterationOptions().contains(choosedElement))
                        {
                            solution->addElementToIterationOptions(choosedElement);
                        }
                    }
                    else
                    {
                        solution->addElementToVisited(choosedElement);
                    }
                }
            }
        }
        selectBestCandidates(problem, beam);
    }
    return bestSolution;
}

problem::SolutionPtr ConstructiveAlgorithm::multistartAlgorithm(problem::Problem& problem, 
    problem::Instance& instance, 
    function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
    selection::SelectorPtr elementSelector,
    StopCriteriaPtr stopCriteria)
{
    problem::SolutionPtr bestSolution = nullptr;
    double bestVal = INFINITY;
    elementSelector->initialize(instance, instance.initializeSolution());
    int iterations = 0, countNoImprovement = 0;
    while (!stopCriteria->shouldStop(iterations, countNoImprovement))
	{
		auto solution = algorithm(problem, instance, elementSelector);
        double val = problem.objectiveValue(solution);
        if (bestSolution == nullptr or val < bestVal) {
            bestSolution = solution;
            bestVal = val;
            countNoImprovement = 0;
        }
        else {
            countNoImprovement++;
        }
        iterations++;

        elementSelector->updateProbabilitiesIteration(instance, solution);
	}
	return bestSolution;
}

problem::SolutionPtr ConstructiveAlgorithm::multistartAlgorithm(problem::Problem& problem,
    problem::Instance& instance,
    function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
    selection::SelectorPtr elementSelector,
    StopCriteriaPtr stopCriteria,
    int numSolutions)
{
    vector<problem::SolutionPtr> ants;
    int iterations = 0, countNoImprovement = 0, bestValue = INFINITY;
    problem::SolutionPtr bestAnt = nullptr;

    while (!stopCriteria->shouldStop(iterations, countNoImprovement)) 
    {
        ants.clear();
        bool improved = false;
        for (int i = 0; i < numSolutions; i++)
        {
            ants.push_back(algorithm(problem, instance, elementSelector));
            double val = ants[i]->getObjectiveValue();
            if (bestAnt == nullptr or val < bestValue) {
                bestAnt = ants[i];
                bestValue = val;
                countNoImprovement = 0;
                improved = true;
            }
        }
        if (!improved)
        {
            countNoImprovement++;
        }
        iterations++;

        for (auto ant : ants) 
        {
            elementSelector->updateProbabilitiesIteration(instance, ant);
        }
    }
    return bestAnt;
}

