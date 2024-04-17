#pragma once

#include "Problem.h"
#include <vector>
#include <memory>
#include <string>

struct KnapsackElement{
	double value;
	int weight;

	KnapsackElement(double value, int weight) : value(value), weight(weight) {};

	bool operator<(const KnapsackElement& other) {
		// Inverted because is a maximization problem
		return -(value / weight) < -(other.value / other.weight);
	};
};


class KnapsackSolution : public problem::Solution<KnapsackElement> {
	std::vector<std::shared_ptr<KnapsackElement>> solution;
	std::vector<std::shared_ptr<KnapsackElement>> visited;
	double currentValue;
	int currentWeight;

public:
	KnapsackSolution(size_t numElements);

	int getCurrentWeight() const;

	virtual void addElementToSolution(std::shared_ptr<KnapsackElement> element);

	virtual void addElementToVisited(std::shared_ptr<KnapsackElement> element);

	virtual double getObjectiveValue();

	virtual bool wasVisited(std::shared_ptr<KnapsackElement> element);

	virtual std::vector<std::shared_ptr<KnapsackElement>> getSolution();

	virtual std::vector<std::shared_ptr<KnapsackElement>> getVisited();
};


class KnapsackInstance : public problem::Instance<KnapsackSolution, KnapsackElement> {
	std::vector<std::shared_ptr<KnapsackElement>> elements;
	int capacity;

public:
	KnapsackInstance() : capacity(0) {};
	
	KnapsackInstance(std::string filename);

	int getCapacity() const;

	std::vector<std::shared_ptr<KnapsackElement>> getElements() const;

	virtual std::shared_ptr<KnapsackSolution> initializeSolution();
};

class KnapsackProblem : public problem::Problem<KnapsackInstance, KnapsackSolution, KnapsackElement> {
	virtual double objectiveValue(KnapsackSolution& solution);
	virtual bool isValid(KnapsackInstance& instance, KnapsackSolution& solution, std::shared_ptr<KnapsackElement> element);
	virtual bool isComplete(KnapsackInstance& instance, KnapsackSolution& solution);
};