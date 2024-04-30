#pragma once

#include <functional>
#include <memory>
#include "Problem.h"


namespace ConstructiveAlgorithm {

    problem::ElementPtr getBestElement(problem::Instance& instance, problem::SolutionPtr solution);

    problem::ElementPtr getElementRandomSelection(problem::Instance& instance, problem::SolutionPtr solution, double alpha, int k, std::function<int(int)> random_distribuiton);

    void selectBestCandidates(problem::Problem& problem, std::vector<problem::SolutionPtr>& solutions);

    problem::SolutionPtr greedyAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<problem::ElementPtr(problem::Instance&, std::shared_ptr<problem::Solution>)> selectionElementAlgorithm);

    problem::SolutionPtr beamsearchAlgorithm(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> selectionElementAlgorithm,
        int beamWidth, 
        int expasionWidth);

    std::shared_ptr<problem::Solution> multistartAlgorithmMaxIterations(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm,
        int numIterations);


}