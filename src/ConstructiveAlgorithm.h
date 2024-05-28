#pragma once

#include <functional>
#include <memory>
#include "Problem.h"
#include "Selector.h"


namespace ConstructiveAlgorithm {

    problem::ElementPtr getBestElement(problem::Instance& instance, problem::SolutionPtr solution, selection::SelectorPtr selector);

    problem::ElementPtr getElementRandomSelection(problem::Instance& instance, problem::SolutionPtr solution, selection::SelectorPtr selector);

    void selectBestCandidates(problem::Problem& problem, std::vector<problem::SolutionPtr>& solutions);

    problem::SolutionPtr greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector);

    problem::SolutionPtr beamsearchAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector, int beamWidth, int expasionWidth);

    std::shared_ptr<problem::Solution> multistartAlgorithmMaxIterations(problem::Problem& problem, 
        problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm,
        int numIterations);

}