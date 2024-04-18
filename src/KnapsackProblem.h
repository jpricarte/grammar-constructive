#pragma once

#include "Problem.h"
#include <vector>
#include <set>
#include <memory>
#include <string>

struct KnapsackElement : public problem::Element {
public:
	double value;
	int weight;

	KnapsackElement(double value, int weight) : value(value), weight(weight) {};

	bool operator<(const KnapsackElement& other) {
		// Inverted because is a maximization problem
		return -(value / weight) < -(other.value / other.weight);
	};
};


class KnapsackSolution : public problem::Solution {
	std::vector<std::shared_ptr<problem::Element>> solution;
	std::set<std::shared_ptr<problem::Element>> visited;
	double currentValue;
	int currentWeight;

public:

	KnapsackSolution(size_t numElements);

	int getCurrentWeight() const;

	std::set<std::shared_ptr<problem::Element>> getVisited();

	int getVisistedSize();

	void addElementToSolution(std::shared_ptr<problem::Element> element) override;

	void addElementToVisited(std::shared_ptr<problem::Element> element) override;

	double getObjectiveValue() override;

	bool wasVisited(std::shared_ptr<problem::Element> element) override;

	std::vector<std::shared_ptr<problem::Element>> getSolution() override;

};

class KnapsackInstance : public problem::Instance {
	std::vector<std::shared_ptr<problem::Element>> elements;
	int capacity;

public:
	KnapsackInstance() : capacity(0) {};
	
	KnapsackInstance(std::string filename);

	int getCapacity() const;

	std::vector<std::shared_ptr<problem::Element>> getElements() const;

	std::shared_ptr<problem::Solution> initializeSolution() override;

	std::vector<std::shared_ptr<problem::Element>> getCandidatesElements(std::shared_ptr<problem::Solution> solution) override;
};

class KnapsackProblem : public problem::Problem {
public:
	KnapsackProblem() {};

	double objectiveValue(std::shared_ptr<problem::Solution> solution) override;

	bool isValid(problem::Instance& instance, std::shared_ptr<problem::Solution> solution, std::shared_ptr<problem::Element> element) override;

	bool isComplete(problem::Instance& instance, std::shared_ptr<problem::Solution> solution) override;
};