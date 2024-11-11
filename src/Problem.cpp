#include "Problem.h"

#include <algorithm>

using namespace problem;

void Solution::addElementToSolution(ElementPtr element)
{
	// Add the color to the solution
	this->solution.push_back(element);
	std::sort(this->candidates.begin(), this->firstVisited, [this](problem::ElementPtr a, problem::ElementPtr b) {
		return this->getElementQuality(a) < this->getElementQuality(b);
	});
}

void Solution::addElementToVisited(ElementPtr element)
{
	// Send the colors to the visited set
	// TODO: create a alternative, receiving the iterator
	auto it = std::find(this->candidates.begin(), this->candidates.end(), element);
	std::rotate(it, it + 1, this->firstVisited);
	this->firstVisited--;
}

void Solution::addElementToIterationOptions(ElementPtr element)
{
	// Add the color to the iteration options vector
	this->options.insert(element);
}

std::vector<ElementPtr> Solution::getCandidatesElements()
{
	// return from the first visited to the end
	std::vector<ElementPtr> candidates{};
	for (auto it = this->candidates.begin(); it != this->firstVisited; ++it)
	{
		auto element = *(it.base());
		if (!this->getIterationOptions().contains(element))
		{
			candidates.push_back(element);
		}
	}
	return candidates;
}

std::set<ElementPtr> Solution::getIterationOptions()
{
	// Returns the options saved to be added in the option
	return this->options;
}

void Solution::cleanIterationOptions()
{
	// Clear the iteration options vector
	this->options.clear();
}

int Solution::getSolutionSize() {
	return this->solution.size();
}

int Solution::getVisistedSize()
{
	// get number of elements from the first visited to the end
	return (int) std::distance(this->firstVisited, this->candidates.end());
}
