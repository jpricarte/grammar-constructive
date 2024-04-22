#pragma once

#include <functional>
#include <memory>
#include "Problem.h"


namespace ConstructiveAlgorithm {

    std::shared_ptr<problem::Element> getBestElement(problem::Instance& instance, std::shared_ptr<problem::Solution> solution)
    {
        auto listCandidates = instance.getCandidatesElements(solution);
        return listCandidates.front();
    }

    std::shared_ptr<problem::Element> getElementAlphaSelection(problem::Instance& instance, std::shared_ptr<problem::Solution> solution, double alpha)
    {
        auto listCandidates = instance.getCandidatesElements(solution);
        
        auto element = listCandidates.front();
        if (rand() % 100 < (1 - alpha) * 100) {
			element = listCandidates.at(rand() % listCandidates.size());
		}

        return element;
    }

    void selectBestCandidates(problem::Problem& problem, std::vector<std::shared_ptr<problem::Solution>>& solutions, std::vector<std::shared_ptr<problem::Solution>>& candidates)
    {
        if (candidates.empty())
            return;

        int solutionSize = (int) solutions.size();
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

    std::shared_ptr<problem::Solution> greedyAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<std::shared_ptr<problem::Element> (problem::Instance&, std::shared_ptr<problem::Solution>)> selectionAlgorithm)
    {
        auto solution = instance.initializeSolution();
        while(not problem.isComplete(instance, solution))
        {
            auto choosedElement = selectionAlgorithm(instance, solution);
            if (problem.isValid(instance, solution, choosedElement))
            {
                solution->addElementToSolution(choosedElement);
            }
            solution->addElementToVisited(choosedElement);
        }
        return solution;
    }

    std::shared_ptr<problem::Solution> beamsearchAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<std::shared_ptr<problem::Element>(problem::Instance&, std::shared_ptr<problem::Solution>)> selectionAlgorithm,
        std::function<void (problem::Problem&, std::vector<std::shared_ptr<problem::Solution>>&, std::vector<std::shared_ptr<problem::Solution>>&)> selectionCandidateAlgorithm,
        int beamWidth, int expasionWidth)
    {
        std::vector<std::shared_ptr<problem::Solution>> beam{};
        std::vector<std::shared_ptr<problem::Solution>> candidates{};
        std::shared_ptr<problem::Solution> bestSolution = nullptr;

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
					auto choosedElement = selectionAlgorithm(instance, solution);
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

        if (bestSolution != nullptr) {
            return bestSolution;
        }
        else return nullptr;
    }

}