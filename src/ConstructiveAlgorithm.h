#pragma once

#include <functional>
#include <memory>
#include "Problem.h"
#include "Selector.h"

#define TUNNING 0 // Will return DBL_MAX if the budget is insufficient for iterated

namespace ConstructiveAlgorithm {

    struct StopCriteria {
        int maxIterations;
        int maxNoImprov;
        uint64_t maxBudget;

        bool shouldStop(int numIterations, int numNoImprov);
    };
    using StopCriteriaPtr = std::shared_ptr<StopCriteria>;

    void selectBestCandidates(problem::Problem& problem, std::vector<problem::SolutionPtr>& solutions, int beamWidth);

    problem::SolutionPtr greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector, StopCriteriaPtr stopCriteria);

    problem::SolutionPtr greedyAlgorithm(problem::Instance& instance, problem::SolutionPtr solution, selection::SelectorPtr selector);

    problem::SolutionPtr beamsearchAlgorithm(problem::Problem& problem,
        problem::Instance& instance,
        selection::SelectorPtr selector,
        int beamWidth, int expasionWidth,
        StopCriteriaPtr stopCriteria);

    problem::SolutionPtr multistartAlgorithm(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
        selection::SelectorPtr elementSelector,
        StopCriteriaPtr stopCriteria);

    problem::SolutionPtr multistartAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
        selection::SelectorPtr elementSelector,
        StopCriteriaPtr stopCriteria,
        int numSolutions);

}
