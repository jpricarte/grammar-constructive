#pragma once

#include <functional>
#include <memory>
#include "Problem.h"


namespace ConstructiveAlgorithm {

    problem::ElementPtr getBestElement(problem::Instance& instance, problem::SolutionPtr solution);

    problem::ElementPtr getElementAlphaSelection(problem::Instance& instance, problem::SolutionPtr solution, double alpha);

    void selectBestCandidates(problem::Problem& problem, std::vector<problem::SolutionPtr>& solutions, std::vector<problem::SolutionPtr>& candidates);

    problem::SolutionPtr greedyAlgorithm(problem::Problem& problem, problem::Instance& instance,
        std::function<problem::ElementPtr(problem::Instance&, std::shared_ptr<problem::Solution>)> selectionElementAlgorithm);

    problem::SolutionPtr beamsearchAlgorithm(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::ElementPtr(problem::Instance&, problem::SolutionPtr)> selectionElementAlgorithm,
        std::function<void(problem::Problem&, std::vector<problem::SolutionPtr>&, std::vector<problem::SolutionPtr>&)> selectionCandidateAlgorithm,
        int beamWidth, 
        int expasionWidth);

    std::shared_ptr<problem::Solution> multistartAlgorithmMaxIterations(problem::Problem& problem,
        problem::Instance& instance,
        std::function<problem::SolutionPtr(problem::Problem&, problem::Instance&)> algorithm,
        int numIterations);


}