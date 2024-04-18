#include "KnapsackProblem.h"

#include <fstream>
#include <algorithm>


KnapsackSolution::KnapsackSolution(size_t numElements) : currentValue(0), currentWeight(0)
{
	this->solution = std::vector<std::shared_ptr<problem::Element>>();
	this->solution.reserve(numElements);
	this->visited = std::set<std::shared_ptr<problem::Element>>();
	this->solution.reserve(numElements);
	this->currentValue = 0;
	this->currentWeight = 0;
}

int KnapsackSolution::getCurrentWeight () const
{
	return this->currentWeight;
}

void KnapsackSolution::addElementToSolution(std::shared_ptr<problem::Element> element)
{
	auto knapsackElement = std::dynamic_pointer_cast<KnapsackElement>(element);
	this->solution.push_back(element);
	this->currentValue += knapsackElement->value;
	this->currentWeight += knapsackElement->weight;
}

void KnapsackSolution::addElementToVisited(std::shared_ptr<problem::Element> element)
{
	this->visited.insert(element);
}

double KnapsackSolution::getObjectiveValue()
{
	return this->currentValue;
}

std::set<std::shared_ptr<problem::Element>> KnapsackSolution::getVisited()
{
	return this->visited;
}

int KnapsackSolution::getVisistedSize()
{
	return this->visited.size();
}

bool KnapsackSolution::wasVisited(std::shared_ptr<problem::Element> element)
{
	return this->visited.contains(element);
}

std::vector<std::shared_ptr<problem::Element>> KnapsackSolution::getSolution()
{
	return this->solution;
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
			double value;
			int weight;
			file >> value >> weight;
			auto element = std::make_shared<KnapsackElement>(value, weight);
			this->elements.push_back(element);
		}
	}
	std::sort(this->elements.begin(), this->elements.end());
}

int KnapsackInstance::getCapacity() const
{
	return this->capacity;
}

std::vector<std::shared_ptr<problem::Element>> KnapsackInstance::getElements() const
{
	return this->elements;
}

std::shared_ptr<problem::Solution> KnapsackInstance::initializeSolution()
{
	return std::make_shared<KnapsackSolution>(elements.size());
}


std::vector<std::shared_ptr<problem::Element>> KnapsackInstance::getCandidatesElements(std::shared_ptr<problem::Solution> solution)
{
	auto listCandidates = std::vector<std::shared_ptr<problem::Element>>();

	for (auto element : this->elements)
	{
		if (not std::dynamic_pointer_cast<KnapsackSolution>(solution)->wasVisited(element))
		{
			listCandidates.push_back(element);
		}
	}
	return listCandidates;
}


double KnapsackProblem::objectiveValue(std::shared_ptr<problem::Solution> solution)
{
	double value = 0;
	for (auto& genericElement : solution->getSolution())
	{
		auto element = std::static_pointer_cast<KnapsackElement>(genericElement);
		value += element->value;
	}

	return value;
}

bool KnapsackProblem::isValid(problem::Instance& instance, std::shared_ptr<problem::Solution> solution, std::shared_ptr<problem::Element> element)
{
	auto knapsackElement = std::static_pointer_cast<KnapsackElement>(element);
	auto knapsackSolution = std::static_pointer_cast<KnapsackSolution>(solution);
	auto& knapsackInstance = (KnapsackInstance&) instance;

	return knapsackSolution->getCurrentWeight() + knapsackElement->weight <= knapsackInstance.getCapacity();
}

bool KnapsackProblem::isComplete(problem::Instance& instance, std::shared_ptr<problem::Solution> solution)
{
	auto knapSackSolution = std::dynamic_pointer_cast<KnapsackSolution>(solution);
	auto& knapsackInstance = (KnapsackInstance&) instance;
	return knapsackInstance.getElements().size() == knapSackSolution->getVisistedSize();
}