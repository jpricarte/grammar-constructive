#include "StaticSelector.h"

using namespace selection;

template<typename T>
T GreedySelector<T>::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
	auto listCandidates = instance.getCandidatesElements(solution);

    if (listCandidates.empty()) {
		return nullptr;
    }
    else {
        return listCandidates.front();
    }
}

template<typename T>
T RandomSelector<T>::selectElement(problem::Instance& instance, problem::SolutionPtr solution)
{
    auto listCandidates = instance.getCandidatesElements(solution);

    if (listCandidates.empty()) {
        return nullptr;
    }

    auto element = listCandidates.front();
    if (rand() % 100 < (1 - this->alphaValue) * 100) {
        int max = (this->kValue <= 0 || this->kValue >= listCandidates.size()) ? listCandidates.size() - 1 : this->kValue;
        this->distribuition = uniform_int_distribution<int>(0, max);
        int pos = distribution(this->generator);
        element = listCandidates.at(pos);
    }

    return element;
}
