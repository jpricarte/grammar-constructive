#include "KnapsackProblem.h"

#include <fstream>

KnapsackSolution::KnapsackSolution(size_t numElements) : currentValue(0), currentWeight(0)
{
	this->solution = std::vector<std::shared_ptr<KnapsackElement>>();
	this->solution.reserve(numElements);
	this->visited = std::vector<std::shared_ptr<KnapsackElement>>();
	this->solution.reserve(numElements);
}

int KnapsackSolution::getCurrentWeight () const
{
	return this->currentWeight;
}

void KnapsackSolution::addElementToSolution(std::shared_ptr<KnapsackElement> element)
{
	this->solution.push_back(element);
	this->currentValue += element->value;
	this->currentWeight += element->weight;
}

void KnapsackSolution::addElementToVisited(std::shared_ptr<KnapsackElement> element)
{
	this->visited.push_back(element);
}

double KnapsackSolution::getObjectiveValue()
{
	return this->currentValue;
}

bool KnapsackSolution::wasVisited(std::shared_ptr<KnapsackElement> element)
{
	return false;
}

std::vector<std::shared_ptr<KnapsackElement>> KnapsackSolution::getSolution()
{
	return this->solution;
}

std::vector<std::shared_ptr<KnapsackElement>> KnapsackSolution::getVisited()
{
	return this->visited;
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
			this->elements.push_back(std::make_shared<KnapsackElement>(element));
		}
	}
}

int KnapsackInstance::getCapacity() const
{
	return this->capacity;
}

std::vector<std::shared_ptr<KnapsackElement>> KnapsackInstance::getElements() const
{
	return this->elements;
}

std::shared_ptr<KnapsackSolution> KnapsackInstance::initializeSolution()
{
	return std::make_shared<KnapsackSolution>(KnapsackSolution(this->elements.size()));
}

double KnapsackProblem::objectiveValue(KnapsackSolution& solution)
{
	double value = 0;
	for (auto& element : solution.getSolution())
	{
		value += element->value;
	}

	return value;
}

bool KnapsackProblem::isValid(KnapsackInstance& instance, KnapsackSolution& solution, std::shared_ptr<KnapsackElement> element)
{
	return solution.getCurrentWeight() + element->weight <= instance.getCapacity();
}

bool KnapsackProblem::isComplete(KnapsackInstance& instance, KnapsackSolution& solution)
{
	return instance.getElements().size() == solution.getVisited().size();
}
