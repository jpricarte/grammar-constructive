#include "Problem.h"

#include <algorithm>

using namespace problem;

void Solution::addElementToSolution(ElementPtr element)
{
	// Add the color to the solution
	this->solution.push_back(element);
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
	return std::vector<ElementPtr>(this->candidates.begin(), this->firstVisited);
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

int Solution::getVisistedSize()
{
	// get number of elements from the first visited to the end
	return std::distance(this->firstVisited, this->candidates.end());
}
