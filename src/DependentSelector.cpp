#include "DependentSelector.h"
#include <algorithm>

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