#pragma once

#include <functional>
#include <memory>
#include "Problem.h"


namespace ConstructiveAlgorithm {

    std::shared_ptr<problem::Element> getBestElement(problem::Instance& instance, std::shared_ptr<problem::Solution> solution);

    std::shared_ptr<problem::Element> getElementAlphaSelection(problem::Instance& instance, std::shared_ptr<problem::Solution> solution, double alpha);

    void selectBestCandidates(problem::Problem& problem, std::vector<std::shared_ptr<problem::Solution>>& solutions, std::vector<std::shared_ptr<problem::Solution>>& candidates);

    std::shared_ptr<problem::Solution> greedyAlgorithm(problem::Problem& problem, problem::Instance& instance, 
        std::function<std::shared_ptr<problem::Element>(problem::Instance&, std::shared_ptr<problem::Solution>)> selectionElementAlgorithm);

    std::shared_ptr<problem::Solution> beamsearchAlgorithm(problem::Problem& problem, 
        problem::Instance& instance,
        std::function<std::shared_ptr<problem::Element>(problem::Instance&, std::shared_ptr<problem::Solution>)> selectionElementAlgorithm,
        std::function<void(problem::Problem&, std::vector<std::shared_ptr<problem::Solution>>&, std::vector<std::shared_ptr<problem::Solution>>&)> selectionCandidateAlgorithm,
        int beamWidth, 
        int expasionWidth);

    std::shared_ptr<problem::Solution> multistartAlgorithmMaxIterations(problem::Problem& problem,
        problem::Instance& instance,
        std::function<std::shared_ptr<problem::Solution>(problem::Problem&, problem::Instance&)> algorithm,
        int numIterations);

}