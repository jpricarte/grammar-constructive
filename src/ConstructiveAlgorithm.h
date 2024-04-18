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

    double greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, 
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
        return solution->getObjectiveValue();
    }
}