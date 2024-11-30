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
        int max = (int)(listCandidates.size() * this->kValue);
        if (max < 1)
            max = 1;
        if (this->kValue <= 0 || this->kValue >= 1)
            max = listCandidates.size() - 1;
        this->distribution = std::uniform_int_distribution<int>(1, static_cast<int>(max));
        int pos;
        pos = (listCandidates.size() == 1) ? 0 : distribution(this->generator);
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
