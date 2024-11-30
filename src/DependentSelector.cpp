#include "DependentSelector.h"
#include "ConstructiveAlgorithm.h"
#include <algorithm>
#include <iostream>

void selection::PheromoneSelector::initialize(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);
    for (auto element : listCandidates)
    {
        this->pheromones[element] = 1.0;
    }
}

void selection::PheromoneSelector::updateProbabilitiesInternal(problem::Instance& instance, problem::SolutionPtr solution)
{ /* Do Nothing */ }

void selection::PheromoneSelector::updateProbabilitiesIteration(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = solution->getSolution();
    auto invValue = 1 / solution->getObjectiveValue();
    for (auto& [element, pheromone] : this->pheromones)
    {
        double deltaPheromone = (std::find(listCandidates.begin(), listCandidates.end(), element) != listCandidates.end()) ? invValue : 0;
        pheromone = pheromone * (1 - this->evaporationRate) + this->evaporationRate * deltaPheromone;
    }
}

problem::ElementPtr selection::PheromoneSelector::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    this->probabilities.clear();

    auto listCandidates = instance.getCandidatesElements(solution);
    if (listCandidates.empty())
    {
        return nullptr;
    }

    std::discrete_distribution<> distribution;
    for (auto element : listCandidates)
    {
        if (not this->pheromones.contains(element)) {
            this->pheromones[element] = 1.0;
        }

        this->probabilities.push_back(pow(this->pheromones[element], this->alpha) * pow(solution->getElementQuality(element), this->beta));
    }

    distribution = std::discrete_distribution<>(this->probabilities.begin(), this->probabilities.end());
    int pos = distribution(this->generator);

    return listCandidates.at(pos);
}

selection::PilotSelector::PilotSelector(int KValue) : kValue(KValue)
{
    baseSelector = std::make_shared<GreedySelector>();
}

problem::ElementPtr selection::PilotSelector::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    double bestVal = INFINITY;
    problem::ElementPtr bestElement = nullptr;
    auto listCandidates = solution->getCandidatesElements();
    int minRange;
    if (kValue == 0)
        minRange = (int)listCandidates.size();
    else
        minRange = std::min(kValue, (int)listCandidates.size());

    for (int i = 0; i < minRange; i++)
    {
        auto candidate = solution->getCandidatesElements().at(i);
        auto possibleSolution = solution->clone();
        possibleSolution->addElementToSolution(candidate);
        possibleSolution = ConstructiveAlgorithm::greedyAlgorithm(instance, possibleSolution, baseSelector);
        double val = possibleSolution->getObjectiveValue();
        if (val < bestVal)
        {
            bestVal = val;
            bestElement = candidate;
        }
    }

    return bestElement;
}
