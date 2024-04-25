#include "KnapsackProblem.h"

#include <fstream>
#include <algorithm>

KnapsackSolution::KnapsackSolution(size_t numElements) : currentValue(0), currentWeight(0)
{
	this->solution = std::vector<problem::ElementPtr>();
	this->solution.reserve(numElements);
	this->visited = std::set<problem::ElementPtr>();
	this->solution.reserve(numElements);
	this->currentValue = 0;
	this->currentWeight = 0;
}

int KnapsackSolution::getCurrentWeight () const
{
	return this->currentWeight;
}

void KnapsackSolution::addElementToSolution(problem::ElementPtr element)
{
	auto knapsackElement = std::dynamic_pointer_cast<KnapsackElement>(element);
	this->solution.push_back(element);
	this->currentValue += knapsackElement->value;
	this->currentWeight += knapsackElement->weight;
}

void KnapsackSolution::addElementToVisited(problem::ElementPtr element)
{
	this->visited.insert(element);
}

double KnapsackSolution::getObjectiveValue()
{
	return this->currentValue;
}

std::set<problem::ElementPtr> KnapsackSolution::getVisited()
{
	return this->visited;
}

int KnapsackSolution::getVisistedSize()
{
	return (int) this->visited.size();
}

bool KnapsackSolution::wasVisited(problem::ElementPtr element)
{
	return this->visited.contains(element);
}

std::vector<problem::ElementPtr> KnapsackSolution::getSolution()
{
	return this->solution;
}

problem::SolutionPtr KnapsackSolution::clone()
{
	auto newSolution = std::make_shared<KnapsackSolution>(this->solution.size());
	for (auto element : this->solution)
	{
		newSolution->addElementToSolution(element);
	}
	for (auto element : this->visited)
	{
		newSolution->addElementToVisited(element);
	}
	return newSolution;
}


KnapsackInstance::KnapsackInstance(std::string filepath)
{
	this->capacity = 0;
	std::ifstream file(filepath);
	std::string line;
	if (file.is_open()) {
		// read empty line
		getline(file, line);
		// read number of elements
		int numElements;
		file >> numElements;
		// Set size of elements
		this->elements.reserve(numElements);
		// read capacity
		file >> this->capacity;
		// read empty line
		getline(file, line);
		while (not file.eof()) {
			if (file.peek() == '#') {
				getline(file, line);
				continue;
			}
			double value;
			int weight;
			file >> value >> weight;
			auto element = std::make_shared<KnapsackElement>(value, weight);
			this->elements.push_back(element);
		}
	}
	sortItems();
}

int KnapsackInstance::getCapacity() const
{
	return this->capacity;
}

std::vector<problem::ElementPtr> KnapsackInstance::getElements() const
{
	return this->elements;
}

void KnapsackInstance::sortItems()
{
	std::sort(this->elements.begin(), this->elements.end(), [](problem::ElementPtr a, problem::ElementPtr b) {
		auto elementA = std::dynamic_pointer_cast<KnapsackElement>(a);
		auto elementB = std::dynamic_pointer_cast<KnapsackElement>(b);
		return *elementA < *elementB;
	});
}

problem::SolutionPtr KnapsackInstance::initializeSolution()
{
	return std::make_shared<KnapsackSolution>(elements.size());
}


std::vector<problem::ElementPtr> KnapsackInstance::getCandidatesElements(problem::SolutionPtr solution)
{
	auto listCandidates = std::vector<problem::ElementPtr>();

	for (auto element : this->elements)
	{
		if (not std::dynamic_pointer_cast<KnapsackSolution>(solution)->wasVisited(element))
		{
			listCandidates.push_back(element);
		}
	}
	return listCandidates;
}


double KnapsackProblem::objectiveValue(problem::SolutionPtr solution)
{
	double value = 0;
	for (auto& genericElement : solution->getSolution())
	{
		auto element = std::static_pointer_cast<KnapsackElement>(genericElement);
		value += element->value;
	}

	return value;
}

bool KnapsackProblem::isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element)
{
	auto knapsackElement = std::static_pointer_cast<KnapsackElement>(element);
	auto knapsackSolution = std::static_pointer_cast<KnapsackSolution>(solution);
	auto& knapsackInstance = (KnapsackInstance&) instance;

	return knapsackSolution->getCurrentWeight() + knapsackElement->weight <= knapsackInstance.getCapacity();
}

bool KnapsackProblem::isComplete(problem::Instance& instance, problem::SolutionPtr solution)
{
	auto knapSackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
	auto& knapsackInstance = (KnapsackInstance&) instance;
	return knapsackInstance.getElements().size() <= knapSackSolution->getVisistedSize();
}

bool KnapsackProblem::elementCompleteSolution(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element)
{
	auto knapSackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
	auto& knapsackInstance = (KnapsackInstance&)instance;
	// If the element is the last one, the solution is complete
	return knapsackInstance.getElements().size() == knapSackSolution->getVisistedSize() - 1;
}
