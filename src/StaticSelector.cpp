#include "StaticSelector.h"
#include <iostream>
#include <memory>
using namespace selection;

problem::ElementPtr GreedySelector::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    if (listCandidates.empty()) {
        return nullptr;
    }
    else {
        return listCandidates.front();
    }
}

problem::ElementPtr RandomSelector::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    if (listCandidates.empty()) {
        return nullptr;
    }

    auto element = listCandidates.front();
    if (rand() % 100 < static_cast<int>((1 - this->alphaValue) * 100)) {
        auto max = (this->kValue <= 0 || this->kValue >= listCandidates.size()) ? listCandidates.size() - 1 : this->kValue;
        this->distribution = std::uniform_int_distribution<int>(1, static_cast<int>(max));
        int pos;
        if (listCandidates.size() == 1) pos = 0;
        else pos = distribution(this->generator);
        element = listCandidates.at(pos);
    }
    return element;
}

problem::ElementPtr selection::WeightedRandomSelector::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    if (listCandidates.empty()) {
        return nullptr;
    }     
    std::vector<double> weights;
    for (auto element : listCandidates) 
    {
        weights.push_back(solution->getElementQuality(element));
    }
        
    std::discrete_distribution<> distribution(weights.begin(), weights.end());
    int pos = distribution(this->generator);
    auto element = listCandidates.at(pos);

    return element;
}
