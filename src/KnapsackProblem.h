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
		return -(value) < -(other.value);
	};
};


class KnapsackSolution : public problem::Solution {
	std::vector<problem::ElementPtr> solution;
	std::set<problem::ElementPtr> visited;
	double currentValue;
	int currentWeight;

public:

	KnapsackSolution(size_t numElements);

	int getCurrentWeight() const;

	std::set<problem::ElementPtr> getVisited();

	int getVisistedSize();

	void addElementToSolution(problem::ElementPtr element) override;

	void addElementToVisited(problem::ElementPtr element) override;

	double getObjectiveValue() override;

	bool wasVisited(problem::ElementPtr element) override;

	std::vector<problem::ElementPtr> getSolution() override;

	problem::SolutionPtr clone() override;
};

class KnapsackInstance : public problem::Instance {
	std::vector<problem::ElementPtr> elements;
	int capacity;

public:
	KnapsackInstance() : capacity(0) {};
	
	KnapsackInstance(std::string filename);

	int getCapacity() const;

	std::vector<problem::ElementPtr> getElements() const;

	void sortItems();

	problem::SolutionPtr initializeSolution() override;

	std::vector<problem::ElementPtr> getCandidatesElements(problem::SolutionPtr solution) override;
};

class KnapsackProblem : public problem::Problem {
public:
	KnapsackProblem() {};

	double objectiveValue(problem::SolutionPtr solution) override;

	bool isValid(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;

	bool isComplete(problem::Instance& instance, problem::SolutionPtr solution) override;

	bool elementCompleteSolution(problem::Instance& instance, problem::SolutionPtr solution, problem::ElementPtr element) override;
};