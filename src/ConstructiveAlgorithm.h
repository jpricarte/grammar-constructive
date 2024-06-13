#pragma once

#include <functional>
#include <memory>
#include "Problem.h"
#include "Selector.h"


namespace ConstructiveAlgorithm {

    struct StopCriteria {
        int maxIterations;
        int maxNoImprovementIterations;

        bool shouldStop(int numIterations, int numNoImprov);
    };
    using StopCriteriaPtr = std::shared_ptr<StopCriteria>;

    struct Ant {
        problem::SolutionPtr solution;
        double value;
    };
    using AntPtr = std::shared_ptr<Ant>;

    void selectBestCandidates(problem::Problem& problem, std::vector<problem::SolutionPtr>& solutions);

    problem::SolutionPtr greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector);

    problem::SolutionPtr beamsearchAlgorithm(problem::Problem& problem, problem::Instance& instance, selection::SelectorPtr selector, int beamWidth, int expasionWidth);

    problem::SolutionPtr multistartAlgorithmMaxIterations(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
        selection::SelectorPtr elementSelector,
        StopCriteriaPtr stopCriteria);

    problem::SolutionPtr antColonyAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&, selection::SelectorPtr)> algorithm,
        selection::SelectorPtr elementSelector,
        StopCriteriaPtr stopCriteria,
        int numAnts);

}